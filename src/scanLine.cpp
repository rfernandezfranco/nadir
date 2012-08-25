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
 
#include <QtGui>
#include <QDesktopWidget>
#include "mainWidget.h"
#include "scanLine.h"

ScanLine::ScanLine( QWidget *parent, lineType type, Keyboard *kbd ):
  QWidget( parent )
{
  Qt::WindowFlags flags;
  flags |= Qt::ToolTip;
  flags |= Qt::WindowStaysOnTopHint;
  flags |= Qt::FramelessWindowHint;
  setWindowFlags( flags );
  setAttribute( Qt::WA_ShowWithoutActivating );
  setFocusPolicy( Qt::NoFocus );

  QCoreApplication::setOrganizationName( "siesta" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "nadir" );

  loadSettings();
  step = 2;
  init( type );
  myKbd = kbd;
}

void ScanLine::loadSettings()
{
  QSettings settings;

  settings.beginGroup("Main");
  speed = settings.value( "speed", 30 ).toInt();
  _thickness = settings.value( "thickness", 4 ).toInt();
  escapeCode = settings.value( "escape", 9 ).toInt();
  lineColor = settings.value( "color", "255,0,0" ).toString();
  setStyleSheet( backgroundColor() );
  settings.endGroup();
}

void ScanLine::init( lineType type )
{
  // Set line thickness and line length
  getScreenSize();
  timer = new QTimer(this);
  myType = type;
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
  setStyleSheet( backgroundColor() );
}

QString ScanLine::backgroundColor()
{
  QString s;

  s.clear();
  s.append( "QWidget { background-color: rgb(" );
  s.append( lineColor );
  s.append( ") };" );

  return s;
}

void ScanLine::startScan( void )
{
  loadSettings();

  if(myType == HORIZONTAL)
    resize( _length, _thickness );	
  else
    resize( _thickness, _length );	

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

void ScanLine::setThickness( int t )
{
  _thickness = t;
}

int ScanLine::getThickness( void )
{
  return _thickness;
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
    myKbd->stop();
    myKbd->start();
    y0 = 0;
  }

  move( x0, y0 );
}

void ScanLine::vScan( void )
{
  if( x0 < ( x1 - _thickness ) )
    x0 += step;
  else{
    myKbd->stop();
    myKbd->start();
    x0 = 0;
  }

  move( x0, y0 );
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

