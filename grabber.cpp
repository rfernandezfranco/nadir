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
 
#include "grabber.h"

Grabber::Grabber()
{
  keyString = (char *)malloc( 20 * sizeof(char));
}

bool Grabber::start()
{
  disp = XOpenDisplay(NULL);
  screen = DefaultScreen(disp);

  if( disp == NULL )
    return false;

  XkbSetDetectableAutoRepeat(disp, TRUE, NULL);
  x11_fd = ConnectionNumber(disp);
  XFlush(disp);
  grabbed = false;

  return true;
}

void Grabber::flush()
{
  XFlush(disp);
}

Display *Grabber::getDisplay()
{
  return disp;
}

/* Return 0:No event, 1:Key event, 2:Scape */
unsigned int Grabber::grabEvent()
{
  // Create a File Description Set containing x11_fd
  FD_ZERO(&in_fds);
  FD_SET(x11_fd, &in_fds);

  // Set timer
  tv.tv_usec = 100;
  tv.tv_sec = 0;

  // Wait for X Event or a Timer
  grabbed = (select(x11_fd+1, &in_fds, 0, 0, &tv)) ? true : false;

  XGrabKeyboard(disp, DefaultRootWindow(disp), TRUE, GrabModeAsync,
  GrabModeAsync, CurrentTime);

  // Handle XEvents and flush the input 
  while(XPending(disp))
      XNextEvent(disp, &xe);

  if( grabbed ){
    iKeyCode = xe.xkey.keycode;
    iKeyState = xe.xkey.state;
    iKeyTime = xe.xkey.time;
    iKeyType = xe.type;
/* 
    switch (iKeyType) {
      case KeyPress:
        fprintf(stderr, "Key pressed: ");
        break;
      case KeyRelease:
        fprintf(stderr, "key released: ");
        break;
    };
*/
    keysym = XKeycodeToKeysym(disp, iKeyCode, 0);
    keyString = XKeysymToString(keysym);
    //fprintf(stderr, "%s - %i\n", keyString, iKeyCode );

    XSetInputFocus(disp, PointerRoot, RevertToParent, iKeyTime);
    XSendEvent(disp, xe.xkey.subwindow, FALSE, xe.type, &xe);
    XSync(disp, FALSE);
    XFlush(disp);

    // Exit when pressing escape key
    if( iKeyCode == escapeCode ){
      return 2;
    };
  };

  // Return true when pressing any key buy the escape key
  if( grabbed && iKeyType == KeyPress)
    return 1;
  else
    return 0;
}

void Grabber::stop()
{
  XCloseDisplay(disp);
}

void Grabber::setEscapeCode( int i )
{
  escapeCode = i;
}

void Grabber::move( int x, int y )
{
  XTestFakeMotionEvent( disp, screen, x, y, 10 );
}

void Grabber::click(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
  XTestFakeButtonEvent( disp, 1, False, 250 );
}

void Grabber::doubleClick(void)
{
  XTestFakeButtonEvent( disp, 1, True, 100 );
  XTestFakeButtonEvent( disp, 1, False, 100 );
  XTestFakeButtonEvent( disp, 1, True, 100 );
  XTestFakeButtonEvent( disp, 1, False, 100 );
}

void Grabber::rightClick(void)
{
  XTestFakeButtonEvent( disp, 3, True, 0 );
  XTestFakeButtonEvent( disp, 3, False, 250 );
}

void Grabber::drag(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
}

void Grabber::drop(void)
{
  XTestFakeButtonEvent( disp, 1, False, 0 );
}

Grabber::~Grabber()
{
}

