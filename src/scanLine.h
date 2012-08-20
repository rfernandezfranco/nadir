/*
 * Copyright (C) 2009-2011 Juan Roldan Ruiz <juan.roldan@gmail.com>
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
 
#ifndef SCANLINE_H
#define SCANLINE_H

#include <QMainWindow>
#include <QtGui/QWidget>
#include <QTimer>
#include <QWidget>
#include <X11/Xlib.h>
#include "grabber.h"

enum lineType { HORIZONTAL=0, VERTICAL };

class ScanLine : public QWidget
{
  Q_OBJECT

  public:
    ScanLine( QWidget *parent = 0, lineType type=HORIZONTAL, Grabber *grabber=0 );
    QPixmap *mainPixmap;
    void loadSettings();
    void init( lineType type );
    void getScreenSize();
    void setScreenWidth( int w );
    int getScreenWidth( void );
    void setScreenHeight( int h );
    int getScreenHeight( void );
    void setSpeed( int s );
    int getSpeed( void );
    void setThickness( int t );
    int getThickness( void );
    void startScan( void );
    void stopScan( void );
    int getX( void );
    int getY( void );
    QString backgroundColor();

  protected:
    //void paintEvent( QPaintEvent * );
    bool active;
    bool continuous;
    enum { LEFT=0, RIGHT, DOUBLE, DRAG, DROP } mouseEvent;

  protected slots:
    void hScan ( void );
    void vScan ( void );

  private:
    int x0;
    int x1;
    int y0;
    int y1;
    int _thickness;
    int _length;
    QString lineColor;
    int screenWidth;
    int screenHeight;
    int speed;
    int step;
    QTimer *timer;
    Grabber *kbdGrabber;
    lineType myType;
    int escapeCode;
};

#endif // SCANLINE_H

