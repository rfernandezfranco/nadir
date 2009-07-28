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
//#include <QtGui/QMessageBox>
#include "mainWidget.h"
#include <iostream>

using namespace std;

//class ScanLine;

MainWidget::MainWidget()
{
  ui.setupUi(this);

  connect( ui.autoButton, SIGNAL(clicked()),
      this, SLOT(scan()) );     
  connect( ui.doClickButton, SIGNAL(clicked()),
      this, SLOT(stop()) );     
  connect( ui.confButton, SIGNAL(clicked()),
      this, SLOT(configure()) );
  connect( ui.exitButton, SIGNAL(clicked()),
      this, SLOT(close()) );


  xc = new XevieClass();
  if ( !xc->start() )
    close();

  //setFixedSize( size() );
  hLine = new ScanLine( this, HORIZONTAL, xc );
  vLine = new ScanLine( this, VERTICAL, xc );

  XevieSelectInput(xc->dpy, KeyPressMask );
  scanTimer = new QTimer(this);
  connect(scanTimer, SIGNAL(timeout()), this, SLOT( grabEvent() ));
  speed = 30;

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
  //mouseEvent = DRAG;
  //vLine = new ScanLine( this, VERTICAL );
  //xc->grabEvent();
  state = SCAN1;
  scanTimer->start(speed);
  //changeState();
  /*
    XNextEvent(xc->dpy, &xc->event);
    xc->xcme = (XClientMessageEvent *)&xc->event;
    // for readOnly users, send events back to Xserver immediately
    printf("(%4d)", count++);
    switch(xc->event.type) {
      case KeyPress:
        usleep(xc->_delay);
        printf(" KeyPress\n");
        xc->printKeyEvent (&xc->event);
        ke = (XKeyEvent *)&xc->event;
        if( ke->keycode == 9 )
          //quit = 1;
          stop();
        break;
      case KeyRelease: printf(" KeyRelease\n"); break;
      case ButtonPress: usleep(xc->_delay); printf(" ButtonPress\n");
                        xc->printKeyEvent (&xc->event);
                        break;
      case ButtonRelease: printf(" ButtonRelease\n"); break;
      case MotionNotify: printf(" MotionNotify\n"); break;
      case ClientMessage: printf("ClientMessage: <%s>\n", &xc->xcme->data.b[1]); break;
      default: printf(" unknown event %x\n", xc->event.type); break;
    }
    XevieSendEvent(xc->dpy, &xc->event, XEVIE_UNMODIFIED);
  */
}

void MainWidget::grabEvent()
{
  if( xc->grabEvent() )
    changeState();
}

void MainWidget::stop()
{
  //hLine->stopScan();
  //printf("XevieEnd(xc->dpy) finished \n");
  XevieEnd(xc->dpy);
}

void MainWidget::configure( void )
{
   confWidget = new ConfWidget( this );
   confWidget->show();
}

void MainWidget::about()
{
  //hLine->stopScan();
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
/*
void MainWidget::keyPressEvent( QKeyEvent *event )
{
  QWidget::keyPressEvent(event);

  switch (event->key()) {
     case Qt::Key_Left:
       cout << "LEFT Click fom MainWidget" << endl;
       fflush( stdout );
       break;
  }

 cout << "Click fom MainWidget" << endl;
 fflush( stdout );
}
*/
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
      xc->move( vLine->getX(), hLine->getY() );
      cout << vLine->getX() << "," << hLine->getY();
      xc->click();
      state = SCAN1;
      changeState();
      break;
  };
}

MainWidget::~MainWidget()
{
  XevieEnd(xc->dpy);
  xc->end();
}
