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
#include <QScreen>
#include <QGuiApplication>
#include <QCursor>
#include "mainWidget.h"

MainWidget::MainWidget()
{
  ui.setupUi(this);
  setWindowFlags(Qt::Window);

  connect(ui.confButton, &QPushButton::clicked,
          this, &MainWidget::configure);
  connect(ui.clickButton, &QPushButton::clicked,
          this, &MainWidget::setEvent);
  connect(ui.dbClickButton, &QPushButton::clicked,
          this, &MainWidget::setEvent);
  connect(ui.rightClickButton, &QPushButton::clicked,
          this, &MainWidget::setEvent);
  connect(ui.dragButton, &QPushButton::clicked,
          this, &MainWidget::setEvent);

  kbd = new Keyboard();
  mouse = new Mouse();
  if ( !kbd->start() || !mouse->start() )
    close();

  SettingsData settings;

  mic = new Microphone( &settings, this );

  waiting = false;
  trayIcon = NULL;
  firstConf = true;

  hLine = new ScanLine( nullptr, HORIZONTAL, kbd );
  vLine = new ScanLine( nullptr, VERTICAL, kbd );

  scanTimer = new QTimer(this);
  connect(scanTimer, &QTimer::timeout, this, &MainWidget::grabEvent);

  QCoreApplication::setOrganizationName( ORGANIZATION_NAME );
  QCoreApplication::setOrganizationDomain( ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName( APPLICATION_NAME );

  loadInitialSettings();
  loadSettings();
  if(showTrayIcon)
      createTrayIcon();
  scan();

  if( hidePointer )
    kbd->move( getScreenWidth(), getScreenHeight() );

}

void MainWidget::loadInitialSettings()
{
  QSettings settings;

  settings.beginGroup("mainWidget");
  resize( settings.value( "size", QSize( 770, 67 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  settings.endGroup();
}

void MainWidget::loadSettings()
{
  QSettings settings;

  settings.beginGroup("mainWidget");
  resize( settings.value( "size", QSize( 770, 67 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  showTrayIcon = settings.value( "systray", 1 ).toBool();
  settings.endGroup();

  settings.beginGroup("Main");
  speed = settings.value( "speed", 30 ).toInt();
  setMode( settings.value( "mode", 0 ).toInt() );
  continuous = settings.value( "continuous", 0 ).toBool();
  escapeCode = settings.value( "escape", 9 ).toInt();
  ui.clickButton->setChecked( !settings.value( "click", 0 ).toBool() );
  ui.dbClickButton->setChecked( settings.value( "click", 0 ).toBool() );
  hidePointer = settings.value( "hide", 0 ).toBool();
  setDefaultEvent( settings.value( "click", 0 ).toInt() );
  setThreshold( settings.value( "audioThreshold", 0 ).toInt() );
  waitTime = settings.value( "waitTime", 1000 ).toInt();
  confirmOnExit = settings.value( "confirmOnExit", 1).toBool();
  int loadedMouseButton = settings.value("mouseButton", 1).toInt();
  settings.endGroup();

  getScreenSize();

  if(mode == MOUSE)
      mouse->setButtonCode(loadedMouseButton);
  else
      mouse->setButtonCode(0);

  kbd->setEscapeCode( escapeCode );
  hLine->loadSettings();
  vLine->loadSettings();

  forceClosing = false;
}

void MainWidget::reloadSettings()
{
  Mode oldMode = mode;
  loadSettings();

  // Any case but continuing in mic mode
  if( !( oldMode == mode && mode == MIC ) ){
    stop();
    scan();
  };

  // From mic mode to other modes
  if( oldMode != mode && mode != MIC ){
    mic->capture(false);
  };

  // System tray icon
  if(showTrayIcon)
      (trayIcon==NULL) ? createTrayIcon() : trayIcon->show();

  if(!showTrayIcon && trayIcon)
      trayIcon->hide();
}

void MainWidget::saveSettings()
{
  QSettings settings;

  settings.beginGroup( "mainWidget" );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();
}

void MainWidget::setThreshold( int i )
{
  threshold = i;
}

void MainWidget::micEvent( double d )
{
  if(mode != MIC)
    return;
  if( d > threshold  && !waiting ) {
    changeState();
    waiting = true;
    QTimer::singleShot( waitTime, this, SLOT(endWait()) );
  };
}

void MainWidget::endWait()
{
  waiting = false;
}

void MainWidget::getScreenSize()
{
  QScreen *screen = QGuiApplication::primaryScreen();
  if(screen) {
    QSize size = screen->geometry().size();
    setScreenWidth(size.width());
    setScreenHeight(size.height());
  }
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
      scanTimer->start( speed );
      break;
    case MIC:
      mic->capture( true );
      scanTimer->start( speed );
      break;
    case MOUSE:
       scanTimer->start( speed );
       break;
  };
}

void MainWidget::grabEvent()
{
  unsigned int e = 0;
  if(mode == KEY) {
      e = kbd->grabKeyEvent();
  } else if(mode == MOUSE) {
      if (state == WAIT_FOR_DROP_TRIGGER) {
          e = mouse->snoopButtonEvent();
      } else {
          e = mouse->grabButtonEvent();
      }
  }

  if(e) {
      if (mode == MOUSE && state == WAIT_FOR_DROP_TRIGGER) {
          QSettings settings;
          settings.beginGroup("Main");
          int loadedMouseButton = settings.value("mouseButton", 1).toInt();
          mouse->setButtonCode(loadedMouseButton);
      }
      changeState();
  }
}

void MainWidget::stop()
{
  switch( mode ){
    case KEY:
      scanTimer->stop();
      break;
    case MIC:
      mic->capture(false);
      break;
    case MOUSE:
       scanTimer->stop();
       break;
  };
}

void MainWidget::configure( void )
{
  confWidget = new ConfWidget( this, mic, kbd, mouse );
  confWidget->show();
}

void MainWidget::changeState()
{
  switch( state ){
    case STOP:
      state = SCAN1;
      break;

    case SCAN1:
      kbd->snoop();
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
      kbd->noSnoop();
      hLine->hide();
      vLine->hide();
      kbd->move( vLine->getX(), hLine->getY() );
      doEvent();
      if (mouseEvent == DROP) {
        state = WAIT_FOR_DROP_TRIGGER;
      } else {
        state = (continuous) ? SCAN1 : STOP;
        kbd->snoop();
        changeState();
      }
      break;

    case WAIT_FOR_DROP_TRIGGER:
      if (mode == MOUSE)
        mouse->snoop();
      else
        kbd->snoop();
      state = SCAN1;
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
  switch( i ){
    case 0:
      mode = KEY;
      break;

    case 1:
      mode = MIC;
      break;
    case 2:
      mode = MOUSE;
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
  int oldButton = 0;
  if(mode == MOUSE && mouse){
      oldButton = mouse->getButtonCode();
  }

  // If the pointer is over Nadir's control panel, this click is meant to
  // activate one of the UI buttons. Use programmatic click for this.
  QPoint globalPos = QCursor::pos();
  if(rect().contains(mapFromGlobal(globalPos))){
      QWidget* widget = QApplication::widgetAt(globalPos);
      if (widget) {
          QPushButton* button = qobject_cast<QPushButton*>(widget);
          if (button)
              button->click();
      }
      return;
  }

  switch( mouseEvent ){
    case LEFT:
      kbd->click();
      break;

    case DOUBLE:
      kbd->doubleClick();
      break;

    case RIGHT:
      kbd->rightClick();
      mouseEvent = defaultEvent;
      checkDefaultEventButton();
      break;

    case DRAG:
      // Ungrab the mouse to allow snooping for the drop trigger
      if(mode == MOUSE && mouse)
        mouse->setButtonCode(0);
      kbd->drag();
      mouseEvent = DROP;
      break;

    case DROP:
      kbd->drop();
      mouseEvent = defaultEvent;
      checkDefaultEventButton();
      break;
  };

  kbd->flush();

  if( hidePointer )
    kbd->move( getScreenWidth(), getScreenHeight() );
}

void MainWidget::checkDefaultEventButton()
{
  // Manually clear all check marks before setting the one
  // corresponding to the default event. The automatic exclusivity of
  // the buttons only applies when they are clicked by the user, not
  // when their state is changed programmatically.
  ui.clickButton->setChecked(false);
  ui.dbClickButton->setChecked(false);
  ui.rightClickButton->setChecked(false);
  ui.dragButton->setChecked(false);

  switch (defaultEvent) {
    case LEFT:
      ui.clickButton->setChecked(true);
      break;
    case DOUBLE:
      ui.dbClickButton->setChecked(true);
      break;
    default:
      break;
  }
}

void MainWidget::closeEvent(QCloseEvent *e)
{
  saveSettings();

  if( forceClosing ) {
    mic->capture( false );
    e->accept();
    exit(0);
  }
  else {
    if(confirmOnExit){
      QMessageBox msgBox;
      msgBox.setWindowTitle("Nadir");
      msgBox.setText(tr("Are you sure you want to exit?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      switch( msgBox.exec() ){
        case QMessageBox::Yes:
          mic->capture( false );
          e->accept();
          exit(0);
          break;

        case QMessageBox::No:
          e->ignore();
          break;
      };
    }
    else
      exit(0);
  }
}

void MainWidget::createTrayIcon()
{
    // Actions
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &MainWidget::showNormal);
    restoreAction->setIcon(QIcon(":/images/images/nadir.png"));

    configAction = new QAction(tr("&Settings"), this);
    connect(configAction, &QAction::triggered, this, &MainWidget::configure);
    configAction->setIcon(QIcon(":/images/images/menu-configure.png"));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, &QAction::triggered, this, &MainWidget::close);
    quitAction->setIcon(QIcon(":/images/images/menu-quit.png"));

    // Menu
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(configAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    // Icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/images/nadir.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("Nadir Multimodal Screen Scanner"));

    // Signals
    connect(trayIcon, &QSystemTrayIcon::activated,
                 this, &MainWidget::iconActivated);

    // Show
    trayIcon->show();
}

void MainWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        (isVisible()) ? hide() : showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

MainWidget::~MainWidget()
{
  if(mouse){
      mouse->stop();
      delete mouse;
  }
  if(kbd){
      kbd->stop();
      delete kbd;
  }
}

