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
#include <QSettings>
#include <QColorDialog>
#include "confWidget.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

ConfWidget::ConfWidget( QWidget *parent ):
  QWidget( parent )
{
  setWindowFlags( Qt::Window );
  ui.setupUi(this);

  // Temporally remove Camera Tab
  ui.tabWidget->removeTab(4);

  connect( ui.colorButton, SIGNAL(clicked()),
      this, SLOT(setColor()) );

  connect( ui.cancelButton, SIGNAL(clicked()),
      this, SLOT(close()) );

  connect( ui.saveButton, SIGNAL(clicked()),
      this, SLOT(save()) );

  connect( this, SIGNAL(closing()),
      parentWidget(), SLOT(loadSettings()) );

  QCoreApplication::setOrganizationName( "Nadir" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "Nadir" );

  loadSettings();
}


void ConfWidget::loadSettings()
{
  QSettings settings;

  settings.beginGroup( "Main" );
  ui.speedSlider->setValue( settings.value( "speed" ).toInt() );
  ui.thickBox->setValue( settings.value( "thickness" ).toInt() );
  ui.continuousBox->setChecked( settings.value( "continuous" ).toBool() );
  lineColor.clear();
  lineColor.append( settings.value( "color", "255,0,0").toString() );
  ui.colorButton->setStyleSheet( backgroundColor() );
  settings.endGroup();

  settings.beginGroup("confWidget");
  resize( settings.value( "size", QSize( 770, 670 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  settings.endGroup();
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
  settings.setValue( "color", lineColor );
  settings.endGroup();

  settings.beginGroup( "confWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();

  emit closing();
  close();
}

void ConfWidget::setColor()
{
  QRgb rgb = QColorDialog::getRgba(); 
  QString s;

  lineColor.clear();
  lineColor.append( s.setNum(qRed(rgb)) );
  lineColor.append( "," );
  lineColor.append( s.setNum(qGreen(rgb)) );
  lineColor.append( "," );
  lineColor.append( s.setNum(qBlue(rgb)) );

  ui.colorButton->setStyleSheet( backgroundColor() );
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

