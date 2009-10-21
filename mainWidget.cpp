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
#include "mainWidget.h"
#include <iostream>

using namespace std;

MainWidget::MainWidget()
{
  ui.setupUi(this);

  connect( ui.confButton, SIGNAL(clicked()),
      this, SLOT(configure()) );
  connect( ui.exitButton, SIGNAL(clicked()),
      this, SLOT(close()) );

  kbd = new Grabber();
  if ( !kbd->start() )
    close();

  hLine = new ScanLine( this, HORIZONTAL, kbd );
  vLine = new ScanLine( this, VERTICAL, kbd );

  scanTimer = new QTimer(this);
  connect(scanTimer, SIGNAL(timeout()), this, SLOT( grabEvent() ));

  QCoreApplication::setOrganizationName( "Nadir" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "Nadir" );
  loadSettings();
  scan();
}

 void MainWidget::loadSettings()
 {
   QSettings settings;

   settings.beginGroup("Main");
   //restoreState( settings.value( "wm_state" ).toByteArray() );
   resize( settings.value( "size", QSize( 770, 67 ) ).toSize() );
   move( settings.value( "pos" ).toPoint() );
   speed = settings.value( "speed", 30 ).toInt();
   continuous = settings.value( "continuous", 1 ).toBool();
   active = settings.value( "active", 1 ).toBool();
   settings.endGroup();

   hLine->loadSettings();
   vLine->loadSettings();
 }

 void MainWidget::saveSettings()
 {
   QSettings settings;

   settings.beginGroup( "Main" );
   //settings.setValue( "wm_state", saveState() );
   settings.setValue( "size", size() );
   settings.setValue( "pos", pos() );
   settings.setValue( "speed", speed );
   int c = ( continuous==true ) ? 1 : 0;
   settings.setValue( "continuous", c );
   int a = ( active==true ) ? 1 : 0;
   settings.setValue( "active", a );
   settings.endGroup();
 }

void MainWidget::getScreenSize()
{
  QDesktopWidget *desk = QApplication::desktop();
  setScreenWidth( desk->width() );
  setScreenHeight( desk->height() );
}

void MainWidget::setScreenWidth( int w )
{
  screenWidth = w;
}

int MainWidget::getScreenWidth( void )
{
  return screenWidth;
}

void MainWidget::setScreenHeight( int h )
{
  screenHeight = h;
}

int MainWidget::getScreenHeight( void )
{
  return screenHeight;
}

void MainWidget::setSpeed( int s )
{
  speed = s;
}

int MainWidget::getSpeed( void )
{
  return speed;
}

void MainWidget::scan()
{
  state = SCAN1;
  scanTimer->start(speed);
}

void MainWidget::grabEvent()
{
  if( kbd->grabEvent() )
    changeState();
}

void MainWidget::stop()
{
  kbd->stop();
}

void MainWidget::configure( void )
{
  confWidget = new ConfWidget( this );
  confWidget->show();
}

void MainWidget::changeState()
{
  switch( state ){
    case STOP:
      state = SCAN1;
      break;
    case SCAN1:
      hLine->startScan();
      hLine->show();
      state = SCAN2;
      break;
    case SCAN2:
      hLine->stopScan();
      vLine->startScan();
      vLine->show();
      state = EVENT;
      break;
    case EVENT:
      hLine->hide();
      vLine->hide();
      kbd->move( vLine->getX(), hLine->getY() );
      kbd->click();
      state = (continuous) ? SCAN1 : STOP;
      changeState();
      break;
  };
}

void MainWidget::closeEvent(QCloseEvent *e)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle("Nadir");
  msgBox.setText("Seguro que desea salir?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);

  switch( msgBox.exec() ){
    case QMessageBox::Yes:
      saveSettings();
      e->accept();
      break;
    case QMessageBox::No:
      e->ignore();
      break;
  };
}

MainWidget::~MainWidget()
{
  kbd->stop();
}

