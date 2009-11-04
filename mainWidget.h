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
 
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ui_mainWidget.h"
#include "scanLine.h"
#include "confWidget.h"
#include "grabber.h"
#include "microphone.h"
#include "settings.h"
#include "config.h"

#define WAIT_TIME 1000

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
    enum State { STOP=0, SCAN1, SCAN2, EVENT };
    enum Mode { KEY=0, MIC };
    void checkDefaultEventButton();
    void setMode( int i );

  public slots:
    void configure( void );
    void scan();
    void stop();
    void grabEvent();
    void loadSettings();
    void saveSettings();
    void setDefaultEvent( int i );
    void setEvent();

    void endWait();
    void micEvent(double);
    void setThreshold( int i );

  private:
    Ui::MainWidget ui;
    MouseEvent mouseEvent;
    MouseEvent defaultEvent;
    State state;
    Mode mode;
    Grabber *kbd;
    Microphone *mic;
    double threshold;
    bool waiting;
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
    bool active;
    bool continuous;
    ScanLine *hLine;
    ScanLine *vLine;
    ConfWidget *confWidget;
};

#endif // MAINWIDGET_H
