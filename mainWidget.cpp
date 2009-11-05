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
 
#include <iostream>
#include <QtGui>
#include <QSound>
#include "mainWidget.h"

using namespace std;

MainWidget::MainWidget()
{
  ui.setupUi(this);

  connect( ui.confButton, SIGNAL(clicked()),
      this, SLOT(configure()) );
  connect( ui.exitButton, SIGNAL(clicked()),
      this, SLOT(close()) );
  connect( ui.clickButton, SIGNAL(clicked()),
      this, SLOT(setEvent()) );
  connect( ui.dbClickButton, SIGNAL(clicked()),
      this, SLOT(setEvent()) );
  connect( ui.rightClickButton, SIGNAL(clicked()),
      this, SLOT(setEvent()) );
  connect( ui.dragButton, SIGNAL(clicked()),
      this, SLOT(setEvent()) );

  kbd = new Grabber();
  if ( !kbd->start() )
    close();

  SettingsData settings;

  mic = new Microphone( &settings, this );

  waiting = false;

  hLine = new ScanLine( this, HORIZONTAL, kbd );
  vLine = new ScanLine( this, VERTICAL, kbd );

  scanTimer = new QTimer(this);
  connect(scanTimer, SIGNAL(timeout()), this, SLOT( grabEvent() ));

  QCoreApplication::setOrganizationName( "Nadir" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "Nadir" );

  loadSettings();

  if( hidePointer )
    kbd->move( getScreenWidth(), getScreenHeight() );

  scan();
}

void MainWidget::loadSettings()
{
  QSettings settings;

  settings.beginGroup("Main");
  resize( settings.value( "size", QSize( 770, 67 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  speed = settings.value( "speed", 10 ).toInt();
  setMode( settings.value( "mode", 0 ).toInt() );
  continuous = settings.value( "continuous", 0 ).toBool();
  active = settings.value( "active", 1 ).toBool();
  escapeCode = settings.value( "escape", 9 ).toInt();
  ui.clickButton->setChecked( !settings.value( "click", 0 ).toBool() );
  ui.dbClickButton->setChecked( settings.value( "click", 0 ).toBool() );
  hidePointer = settings.value( "hide", 0 ).toBool();
  setDefaultEvent( settings.value( "click", 0 ).toInt() );
  setThreshold( settings.value( "audioThreshold", 0 ).toInt() );
  settings.endGroup();

  getScreenSize();

  kbd->setEscapeCode( escapeCode );
  hLine->loadSettings();
  vLine->loadSettings();
}

void MainWidget::saveSettings()
{
  QSettings settings;

  settings.beginGroup( "Main" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.setValue( "speed", speed );
  settings.setValue( "escape", escapeCode );
  int i = ( continuous==true ) ? 1 : 0;
  settings.setValue( "continuous", i );
  i = ( active==true ) ? 1 : 0;
  settings.setValue( "active", i );
  settings.endGroup();
}

void MainWidget::setThreshold( int i )
{
  threshold = i;
}

void MainWidget::micEvent( double d )
{
  if( d > threshold ){
    if ( !waiting ) {
      changeState();
      waiting = true;
      QTimer::singleShot( WAIT_TIME, this, SLOT(endWait()) );
    };
  };
}

void MainWidget::endWait()
{
  waiting = false;
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

  switch( mode ){
    case KEY:
        scanTimer->start(speed);
        break;
    case MIC:
        mic->capture( true );
      break;
  };
}

void MainWidget::grabEvent()
{
  switch( mode ){
    case KEY:
      if( kbd->grabEvent() )
        changeState();
      break;
    case MIC:
        //changeState();
      break;
  };
}

void MainWidget::stop()
{
  switch( mode ){
    case KEY:
      kbd->stop();
      break;
    case MIC:
      mic->stop();
      break;
  };
}

void MainWidget::configure( void )
{
  confWidget = new ConfWidget( this, mic );
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
      doEvent();
      state = (continuous) ? SCAN1 : STOP;
      changeState();
      break;
  };
}

void MainWidget::setEvent()
{
  if( ui.clickButton->isChecked() )
    mouseEvent = LEFT; 
  else if( ui.dbClickButton->isChecked() )
    mouseEvent = DOUBLE; 
  else if( ui.rightClickButton->isChecked() )
    mouseEvent = RIGHT; 
  else if( ui.dragButton->isChecked() )
    mouseEvent = DRAG; 
}

void MainWidget::setMode( int i )
{
  mode = ( i==0 ) ? KEY : MIC; 

  switch( mode ){
    case 0:
      mode = KEY;
      break;
    case 1:
      mode = MIC;
      break;
    default:
      mode = KEY;
      break;
  };
}

void MainWidget::setDefaultEvent( int i )
{
  defaultEvent = ( i==0 ) ? LEFT : DOUBLE; 
  mouseEvent = defaultEvent;
}

void MainWidget::doEvent()
{
  switch( mouseEvent ){
    case LEFT:
      kbd->click();
      //cout << "left" << endl;
      break;
    case DOUBLE:
      kbd->doubleClick();
      //cout << "double" << endl;
      break;
    case RIGHT:
      kbd->rightClick();
      mouseEvent = defaultEvent;
      checkDefaultEventButton();
      //cout << "right" << endl;
      break;
    case DRAG:
      kbd->drag();
      mouseEvent = DROP;
      //cout << "drag" << endl;
      break;
    case DROP:
      kbd->drop();
      mouseEvent = defaultEvent;
      checkDefaultEventButton();
      //cout << "drop" << endl;
      break;
    //default:
    //  cout << "nothing" << endl;
  };

  kbd->flush();

  if( hidePointer )
    kbd->move( getScreenWidth(), getScreenHeight() );
}

void MainWidget::checkDefaultEventButton()
{
  switch( defaultEvent ){
    case LEFT:
      ui.clickButton->setChecked( true );
      break;
    case DOUBLE:
      ui.dbClickButton->setChecked( true );
      break;
    default:
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
      mic->capture( false );
      e->accept();
      break;
    case QMessageBox::No:
      e->ignore();
      break;
  };
}

MainWidget::~MainWidget()
{
  stop();
}

