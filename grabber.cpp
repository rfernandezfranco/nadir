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

bool Grabber::grabEvent()
{
  // Create a File Description Set containing x11_fd
  FD_ZERO(&in_fds);
  FD_SET(x11_fd, &in_fds);

  // Set timer
  tv.tv_usec = 250;
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

    switch (iKeyType) {
      case KeyPress:
        fprintf(stderr, "Tecla pulsada: ");
        break;
      case KeyRelease:
        fprintf(stderr, "Tecla soltada: ");
        break;
    };

    keysym = XKeycodeToKeysym(disp, iKeyCode, 0);
    keyString = XKeysymToString(keysym);
    fprintf(stderr, "%s - %i\n", keyString, iKeyCode );

    XSetInputFocus(disp, PointerRoot, RevertToParent, iKeyTime);
    XSendEvent(disp, xe.xkey.subwindow, FALSE, xe.type, &xe);
    XSync(disp, FALSE);
    XFlush(disp);

    // Exit when pressing ESCAPE key
    if( iKeyCode == 9 ){
      fprintf(stderr, "Adios!\n");
      XCloseDisplay( disp );
      exit(10);
    };
  };

  if( grabbed && iKeyType == KeyPress)
    return true;
  else
    return false;
}

void Grabber::stop()
{
  XCloseDisplay(disp);
}

void Grabber::click()
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
  XTestFakeButtonEvent( disp, 1, False, 250 );
}

void Grabber::move( int x, int y )
{
  XTestFakeMotionEvent( disp, screen, x, y, 10 );
}

Grabber::~Grabber()
{
}

bool Grabber::escape()
{
  ke = (XKeyEvent *)&xe;

  if(ke->keycode == 9)
    return true;

  return false;
}

