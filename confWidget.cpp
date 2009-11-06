/*
 * Copyright (C) 2009 Juan Roldan Ruiz <juan.roldan@gmail.com>
 *  
 * This file is part of Nadir.
 * 
 * Nadir is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nadir is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nadir.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <QtGui>
#include <QStringList>
#include <QSettings>
#include <QColorDialog>
#include "confWidget.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define WAIT_TIME 1000

using namespace std;

ConfWidget::ConfWidget( QWidget *parent, Microphone *mic ):
  QWidget( parent )
{
  setWindowFlags( Qt::Window );
  ui.setupUi(this);

  // Temporally remove Mouse and Camera Tabs
  ui.tabWidget->removeTab(2);
  ui.tabWidget->removeTab(3);

  connect( ui.colorButton, SIGNAL(clicked()),
      this, SLOT(setColor()) );

  connect( ui.cancelButton, SIGNAL(clicked()),
      this, SLOT(close()) );

  connect( ui.saveButton, SIGNAL(clicked()),
      this, SLOT(save()) );

  connect( this, SIGNAL(closing()),
      parentWidget(), SLOT(loadSettings()) );

  myMic = mic;
  connect(myMic, SIGNAL(doEvent(double)),
      this, SLOT(updateAudioSlider(double)));

  QCoreApplication::setOrganizationName( "Nadir" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "Nadir" );

  loadSettings();

  ui.audioSlider->setMinimum( -40.0 );
  ui.audioSlider->setMaximum( 1.0 );
  ui.audioSlider->setValue( threshold );
  ui.audioBar->setMinimum( -40.0 );
  ui.audioBar->setMaximum( 1.0 );

  connect( ui.keyMode, SIGNAL(toggled(bool)),
      this, SLOT(setMode(bool)) );

  connect( ui.audioSlider, SIGNAL(valueChanged(int)),
      this, SLOT(setThreshold(int)) );

  waiting = false;
}

void ConfWidget::resetAudioBox()
{
  waiting = false;
}

void ConfWidget::loadSettings()
{
  QSettings settings;

  settings.beginGroup( "Main" );
  ui.speedSlider->setValue( settings.value( "speed" ).toInt() );
  ui.thickBox->setValue( settings.value( "thickness" ).toInt() );
  ui.continuousBox->setChecked( settings.value( "continuous" ).toBool() );
  ui.doubleClickBox->setChecked( settings.value( "click", 0 ).toBool() );
  ui.hidePointerBox->setChecked( settings.value( "hide", 0 ).toBool() );
  ui.micMode->setChecked( settings.value( "mode" ).toBool() );
  lineColor.clear();
  lineColor.append( settings.value( "color", "255,0,0").toString() );
  ui.colorButton->setStyleSheet( backgroundColor() );
  setThreshold( settings.value( "audioThreshold", 0 ).toInt() );
  ui.audioBar->setValue( threshold );
  settings.endGroup();

  settings.beginGroup( "mainWidget" );
  ui.minimizedBox->setChecked( settings.value( "minimized" ).toBool() );
  settings.endGroup();

  settings.beginGroup("confWidget");
  resize( settings.value( "size", QSize( 770, 670 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  settings.endGroup();
 }

void ConfWidget::setThreshold( int i )
{
  threshold = i;
}

//Save settings
void ConfWidget::save()
{
  QSettings settings;

  settings.beginGroup( "Main" );
  settings.setValue( "speed", ui.speedSlider->value() );
  settings.setValue( "thickness",  ui.thickBox->value() );
  int i = ( ui.continuousBox->isChecked() ) ? 1 : 0;
  settings.setValue( "continuous", i );
  i = ( ui.keyMode->isChecked() ) ? 0 : 1;
  settings.setValue( "mode", i );
  i = ( ui.simpleClickBox->isChecked() ) ? 0 : 1;
  settings.setValue( "click", i );
  i = ( ui.hidePointerBox->isChecked() ) ? 1 : 0;
  settings.setValue( "hide", i );
  settings.setValue( "color", lineColor );
  settings.setValue( "audioThreshold", threshold );
  settings.endGroup();

  settings.beginGroup( "mainWidget" );
  i = ( ui.minimizedBox->isChecked() ) ? 1 : 0;
  settings.setValue( "minimized", i );
  settings.endGroup();

  settings.beginGroup( "confWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();

  emit closing();
  close();
}

void ConfWidget::setMode( bool b )
{
/*
  if( b )
    cout << "key mode" << endl;
  else
    cout << "mic mode" << endl;
*/
}

void ConfWidget::updateAudioSlider( double d )
{
  ui.audioBar->setValue( d );
   
  if( d > threshold ){
    if ( !waiting ) {
      ui.audioBox->setStyleSheet( "background-color: #DD0000;" );
      //cout << "evento" << endl;
      waiting = true;
      QTimer::singleShot( WAIT_TIME, this, SLOT(resetAudioBox()) );
    }
  }
  else
    if( !waiting )
      ui.audioBox->setStyleSheet( "background-color: #FFFFFF;" );
      
}

void ConfWidget::setColor()
{
  bool ok;
  QString s;
  QStringList l = lineColor.split( "," ); 
  QRgb rgb = QColorDialog::getRgba( QColor::fromRgb(l.at(0).toInt(),
                                    l.at(1).toInt(),
                                    l.at(2).toInt()).rgb(), &ok ); 
  if( ok ){
    lineColor.clear();
    lineColor.append( s.setNum(qRed(rgb)) );
    lineColor.append( "," );
    lineColor.append( s.setNum(qGreen(rgb)) );
    lineColor.append( "," );
    lineColor.append( s.setNum(qBlue(rgb)) );

    ui.colorButton->setStyleSheet( backgroundColor() );
  };
}

QString ConfWidget::backgroundColor()
{
  QString s;

  s.clear();
  s.append( "background-color: rgb(" );
  s.append( lineColor );
  s.append( ");" );

  return s;
}

void ConfWidget::closeEvent()
{
  QSettings settings;

  settings.beginGroup( "confWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();
}

