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
 
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QSystemTrayIcon>
#include "common.h"
#include "config.h"
#include "confWidget.h"
#include "keyboard.h"
#include "mouse.h"
#include "microphone.h"
#include "scanLine.h"
#include "settings.h"
#include "ui_mainWidget.h"

class QAction;
class QMenu;

class MainWidget : public QWidget
{
  Q_OBJECT

  public:
    MainWidget();
    ~MainWidget();
    void closeEvent( QCloseEvent *e);
    void getScreenSize();
    void setScreenWidth( int w );
    int getScreenWidth( void );
    void setScreenHeight( int h );
    int getScreenHeight( void );
    void setSpeed( int s );
    int getSpeed( void );
    void changeState();
    void doEvent();
    enum MouseEvent { LEFT=0, DOUBLE, RIGHT, DRAG, DROP };
    enum State { STOP=0, SCAN1, SCAN2, EVENT, WAIT_FOR_DROP_TRIGGER };
    enum Mode { KEY=0, MIC, MOUSE };
    void checkDefaultEventButton();
    void setMode( int i );

  public slots:
    void configure( void );
    void scan();
    void stop();
    void grabEvent();
    void loadInitialSettings();
    void loadSettings();
    void saveSettings();
    void reloadSettings();
    void setDefaultEvent( int i );
    void setEvent();
    void endWait();
    void micEvent(double);
    void setThreshold( int i );
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

  private:
    Ui::MainWidget ui;
    MouseEvent mouseEvent;
    MouseEvent defaultEvent;
    State state;
    Mode mode;
    Keyboard *kbd;
    Mouse *mouse;
    Microphone *mic;
    double threshold;
    bool waiting;
    unsigned int waitTime;
    bool forceClosing;
    int x;
    int y;
    int screenWidth;
    int screenHeight;
    int speed;
    int lineWidth;
    int escapeCode;
    XKeyEvent *ke;
    int count;
    QTimer *scanTimer;
    bool hidePointer;
    bool continuous;
    ScanLine *hLine;
    ScanLine *vLine;
    ConfWidget *confWidget;
    QAction *restoreAction;
    QAction *configAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    bool showTrayIcon;
    bool firstConf;
    bool confirmOnExit;
};

#endif // MAINWIDGET_H
