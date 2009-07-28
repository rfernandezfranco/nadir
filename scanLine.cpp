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
#include "scanLine.h"
#include <QDesktopWidget>
#include <iostream>
#include <stdio.h>

using namespace std;

ScanLine::ScanLine( QWidget *parent, lineType type, XevieClass *xc ):
  QWidget( parent )
{
  Qt::WindowFlags flags = Qt::ToolTip;
  flags |= Qt::WindowStaysOnTopHint;
  flags |= Qt::FramelessWindowHint;

  setWindowFlags( flags );
  setFocusPolicy(Qt::StrongFocus);

  speed = 20;
  step = 3;
 _thickness = 10;

  init( type );
  
  xevie = xc;

  //xevie = new XevieClass();
  //if ( !xevie->start() )
  //  close();
    
}

void ScanLine::init( lineType type )
{
  // Set line thickness and line length
  getScreenSize();
  timer = new QTimer(this);

  x0 = y0 = 0;

  // Whether it's horizontal or vertical
  if( type == HORIZONTAL ){
    _length = screenWidth;
    x1 = 0;
    y1 = screenHeight - _thickness;
    connect(timer, SIGNAL(timeout()), this, SLOT( hScan() ));
    resize( _length, _thickness);
  }
  else{
    _length = screenHeight;
    x1 = screenWidth - _thickness;
    y1 = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT( vScan() ));
    resize( _thickness, _length);
  }

  // Set line colour
  setStyleSheet("QWidget { background-color: red; }");

}

void ScanLine::startScan( void )
{
  x0 = 0;
  y0 = 0;
  timer->start(speed);
}

void ScanLine::stopScan( void )
{
  timer->stop();
}

void ScanLine::getScreenSize()
{
  QDesktopWidget *desk = QApplication::desktop();
  setScreenWidth( desk->width() );
  setScreenHeight( desk->height() );
}

void ScanLine::setScreenWidth( int w )
{
  screenWidth = w;
}

int ScanLine::getScreenWidth( void )
{
  return screenWidth;
}

void ScanLine::setScreenHeight( int h )
{
  screenHeight = h;
}

int ScanLine::getScreenHeight( void )
{
  return screenHeight;
}

void ScanLine::setSpeed( int s )
{
  speed = s;
}

int ScanLine::getSpeed( void )
{
  return speed;
}

int ScanLine::getX( void )
{
  return x0;
}

int ScanLine::getY( void )
{
  return y0;
}

void ScanLine::hScan( void )
{
  if( y0 < ( y1 - _thickness ) )
    y0 += step;
  else{
    xevie->end();
    xevie->start();
    y0 = 0;
  }

  move( x0, y0 );
  //xevie->move( x0, y0 );
}

void ScanLine::vScan( void )
{
  if( x0 < ( x1 - _thickness ) )
    x0 += step;
  else{
    xevie->end();
    xevie->start();
    x0 = 0;
  }

  move( x0, y0 );
  //xevie->move( x0, y0 );
}


void ScanLine::about()
{
  QMessageBox::about
    ( this, "Acerca de Nadir",
      "<p>Copyright &copy; 2009. <b>iFreeScanLine</b> is free software; you can "
      "redistribute it and/or  modify it under the terms of the <em>GNU "
      "General Public License</em> as published by the Free Software "
      "Foundation; either version 2 of the License, or (at your option) any "
      "later version.</p>"
      "<p><b>iFreeScanLine</b> is distributed in the hope that it will be useful, but "
      "<em>without any warranty</em>; without even the implied warranty of "
      "<em>merchantibility</em> or <em>fitness for a particular "
      "purpose</em>. See the GNU General Public License for more details.</p>"
      "<p>You should have received a copy of the GNU General Public License "
      "along with <b>mando</b>; if not, contact one of the authors of this "
      "software.</p>"
      "<ul><li>Juan Roldan <a href=\"mailto:juan.roldan@gmail.com\">"
      "juan.roldan@gmail.com</a></li></ul>"
    );
}
/*
void ScanLine::paintEvent( QPaintEvent * )
{
  QPainter painter(this);
  QBrush brush( Qt::red );

  QRegion region( QRect( 0, 0, _length, _thickness ) );

  setMask( region );

  painter.fillRect( 0, 0, this->width(), this->height(), brush );
}
*/

void ScanLine::keyPressEvent( QKeyEvent *event )
{
  switch (event->key()) {
     case Qt::Key_Left:
       cout << "LEFT Click from ScanLine" << endl;
       fflush( stdout );
       break;
  }

  cout << "Click from ScanLine" << endl;
  fflush( stdout );
}

