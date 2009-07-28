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
#include "confWidget.h"
#include <iostream>

//using namespace std;

ConfWidget::ConfWidget( QWidget *parent ):
  QWidget( parent )
{
  setWindowFlags( Qt::Window );
  ui.setupUi(this);
/*
  connect( ui.autoButton, SIGNAL(clicked()),
      this, SLOT(scan()) );     
  connect( ui.confButton, SIGNAL(clicked()),
      this, SLOT(about()) );
*/
  connect( ui.cancelButton, SIGNAL(clicked()),
      this, SLOT(close()) );

  setFixedSize( size() );
}
/*
void ConfWidget::getScreenSize()
{
  QDesktopWidget *desk = QApplication::desktop();
  setScreenWidth( desk->width() );
  setScreenHeight( desk->height() );
}

void ConfWidget::setScreenWidth( int w )
{
  screenWidth = w;
}

int ConfWidget::getScreenWidth( void )
{
  return screenWidth;
}

void ConfWidget::setScreenHeight( int h )
{
  screenHeight = h;
}

int ConfWidget::getScreenHeight( void )a
{
  return screenHeight;
}

void ConfWidget::setSpeed( int s )
{
  speed = s;
}

int ConfWidget::getSpeed( void )
{
  return speed;
}


void ConfWidget::scan()
{
  mouseEvent = DRAG;
  //hLine = new ScanLine( this, HORIZONTAL );
  vLine = new ScanLine( this, VERTICAL );
  vLine->show();
  vLine->startScan();
}

void ConfWidget::about()
{
  vLine->stopScan();
  QMessageBox::about
    ( this, "About Nadir",
      "<p style=\"font-size:smaller;\">Copyright &copy; 2009 Juan Rold&aacute;n Ruiz &lt;<a "
      "href=\"mailto:juan.roldan@gmail.com\">juan.roldan@gmail.com</a>&gt;</p>"
      "<p><br/><br/><b>Nadir</b> is free software; you can "
      "redistribute it and/or  modify it under the terms of the <em>GNU "
      "General Public License</em> as published by the Free Software "
      "Foundation; either version 3 of the License, or (at your option) any "
      "later version.</p>"
      "<p><b>Nadir</b> is distributed in the hope that it will be useful, but "
      "<em>without any warranty</em>; without even the implied warranty of "
      "<em>merchantibility</em> or <em>fitness for a particular "
      "purpose</em>. See the GNU General Public License for more details.</p>"
      "<p>You should have received a copy of the GNU General Public License "
      "along with <b>Nadir</b>; if not, contact one of the authors of this "
      "software.</p>"
      "<ul><li>Juan Roldan <a href=\"mailto:juan.roldan@gmail.com\">"
      "juan.roldan@gmail.com</a></li></ul>" );
}

void ConfWidget::keyPressEvent( QKeyEvent *event )
{
  QWidget::keyPressEvent(event);

  switch (event->key()) {
     case Qt::Key_Left:
       cout << "LEFT Click fom ConfWidget" << endl;
       fflush( stdout );
       break;
  }

 cout << "Click fom ConfWidget" << endl;
 fflush( stdout );
}
*/
