/*
 * Copyright (C) 2009-2012 Juan Roldan Ruiz <juan.roldan@gmail.com>
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
 
#include <QtWidgets>
#include <QColorDialog>
#include <QSettings>
#include <QStringList>
#include "confWidget.h"
#include "mainWidget.h"

ConfWidget::ConfWidget( QWidget *parent, Microphone *mic, Keyboard *kbd ):
  QWidget( parent )
{
  setWindowFlags( Qt::Window );
  ui.setupUi(this);

  // Temporally remove Mouse and Camera Tabs
  ui.tabWidget->removeTab(2);
  ui.tabWidget->removeTab(3);

  connect(ui.colorButton, &QPushButton::clicked,
          this, &ConfWidget::setColor);

  connect(ui.cancelButton, &QPushButton::clicked,
          this, &ConfWidget::close);

  connect(ui.saveButton, &QPushButton::clicked,
          this, &ConfWidget::save);

  if(auto parent = qobject_cast<MainWidget*>(parentWidget()))
      connect(this, &ConfWidget::closing,
              parent, &MainWidget::reloadSettings);

  connect(ui.minimizedBox, &QCheckBox::toggled,
          this, &ConfWidget::minimizedBoxToggled);

  connect(ui.hiddenBox, &QCheckBox::toggled,
          this, &ConfWidget::hiddenBoxToggled);

  connect(ui.changeKeyButton, &QPushButton::clicked,
          this, &ConfWidget::changeKey);

  myMic = mic;
  if( myMic->isCapturing() ){
    connect(myMic, &Microphone::doEvent,
            this, &ConfWidget::updateAudioSlider);
    ui.micWidget->setCurrentIndex( 1 );
  }

  myKbd = kbd;

  QCoreApplication::setOrganizationName( ORGANIZATION_NAME );
  QCoreApplication::setOrganizationDomain( ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName( APPLICATION_NAME );

  loadSettings();

  ui.audioSlider->setMinimum( -40.0 );
  ui.audioSlider->setMaximum( 1.0 );
  ui.audioSlider->setValue( threshold );
  ui.waitSlider->setValue( waitTime );
  ui.audioBar->setMinimum( -40.0 );
  ui.audioBar->setMaximum( 1.0 );

  connect(ui.audioSlider, &QSlider::valueChanged,
          this, &ConfWidget::setThreshold);
  connect(ui.waitSlider, &QSlider::valueChanged,
          this, &ConfWidget::setWaitTime);

  waiting = false;

  ui.pressKeyLabel->setVisible(false);

  // Shrink window to minimum needed size
  resize(0,0);
}

void ConfWidget::minimizedBoxToggled()
{
    if(ui.minimizedBox->isChecked())
        ui.hiddenBox->setChecked(false);
}

void ConfWidget::hiddenBoxToggled()
{
    if(ui.hiddenBox->isChecked())
        ui.minimizedBox->setChecked(false);
}

void ConfWidget::resetAudioBox()
{
  waiting = false;
}

void ConfWidget::loadSettings()
{
  QSettings settings;

  settings.beginGroup( "Main" );
  ui.speedSlider->setValue( settings.value( "speed", 30 ).toInt() );
  ui.thickBox->setValue( settings.value( "thickness", 4 ).toInt() );
  ui.continuousBox->setChecked( settings.value( "continuous", 0 ).toBool() );
  ui.doubleClickBox->setChecked( settings.value( "click", 0 ).toBool() );
  ui.hidePointerBox->setChecked( settings.value( "hide", 0 ).toBool() );
  ui.micMode->setChecked( settings.value( "mode", 0 ).toBool() );
  lineColor.clear();
  lineColor.append( settings.value( "color", "255,0,0").toString() );
  ui.colorButton->setStyleSheet( backgroundColor() );
  setThreshold( settings.value( "audioThreshold", 0 ).toInt() );
  ui.audioBar->setValue( threshold );
  setWaitTime( settings.value( "waitTime", 1000 ).toInt() );
  ui.audioBar->setValue( threshold );
  ui.keyCodeField->setText( settings.value( "keycode", 65).toString() );
  ui.keySymField->setText( settings.value( "keysym", "ESPACIO").toString() );
  ui.confirmOnExitBox->setChecked( settings.value( "confirmOnExit", 1).toBool() );
  settings.endGroup();

  settings.beginGroup( "mainWidget" );
  ui.minimizedBox->setChecked( settings.value( "minimized", 0 ).toBool() );
  ui.hiddenBox->setChecked( settings.value( "hidden", 0 ).toBool() );
  ui.systrayBox->setChecked( settings.value( "systray", 1 ).toBool() );
  settings.endGroup();

  settings.beginGroup("confWidget");
  resize( settings.value( "size", QSize( 770, 670 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  settings.endGroup();
 }

void ConfWidget::setThreshold( int i )
{
  threshold = i;
  QString w;

  w.setNum( i );
  w.append( " dB");
  ui.audioLabel->setText( w );
}

void ConfWidget::setWaitTime( int i )
{
  waitTime = i;
  QString w;
  float t = floor((float)waitTime/10)/100;
  w.setNum( t );
  w.append( trUtf8(" second(s)"));
  ui.waitLabel->setText( w );
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
  settings.setValue( "waitTime", waitTime );
  settings.setValue( "keycode", ui.keyCodeField->text().toInt());
  settings.setValue( "keysym", ui.keySymField->text());
  settings.setValue( "confirmOnExit", ui.confirmOnExitBox->isChecked() ? 1 : 0  );
  settings.endGroup();

  settings.beginGroup( "mainWidget" );
  i = ( ui.minimizedBox->isChecked() ) ? 1 : 0;
  settings.setValue( "minimized", i );
  i = ( ui.hiddenBox->isChecked() ) ? 1 : 0;
  settings.setValue( "hidden", i );
  i = ( ui.systrayBox->isChecked() ) ? 1 : 0;
  settings.setValue( "systray", i );
  settings.endGroup();

  settings.beginGroup( "confWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();

  myKbd->setKeyCode(ui.keyCodeField->text().toInt());

  emit closing();
  close();
}

void ConfWidget::updateAudioSlider( double d )
{
  ui.audioBar->setValue( d );
   
  if( d > threshold ){
    if ( !waiting ) {
      ui.audioBox->setStyleSheet( "background-color: #DD0000;" );
      waiting = true;
      QTimer::singleShot( waitTime, this, SLOT(resetAudioBox()) );
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

void ConfWidget::showAboutText()
{
  showNormal();
  ui.tabWidget->setCurrentWidget(ui.aboutTab);
}

void ConfWidget::closeEvent()
{
  QSettings settings;

  settings.beginGroup( "confWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();
}

void ConfWidget::changeKey()
{
  ui.keyGroupBox->setVisible(false);
  ui.pressKeyLabel->setVisible(true);
  ui.pressKeyLabel->setFocus();
}

void ConfWidget::keyPressEvent(QKeyEvent *e)
{
  if(ui.pressKeyLabel->isVisible()){
    QString s = e->text();
    Key::getSym(e->nativeScanCode(), &s);
    ui.keySymField->setText(s);
    ui.keyCodeField->setText(QString::number(e->nativeScanCode()));

    ui.pressKeyLabel->setVisible(false);
    ui.keyGroupBox->setVisible(true);
  }
}
