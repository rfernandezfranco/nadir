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

  return true;
}


void Grabber::grabEvent()
{
  XGrabKeyboard(disp, DefaultRootWindow(disp), TRUE, GrabModeAsync,
  GrabModeAsync, CurrentTime);
  XNextEvent(disp, &xe);

  iKeyCode = xe.xkey.keycode;
  iKeyState = xe.xkey.state;
  iKeyTime = xe.xkey.time;
  iKeyType = xe.type;

  Window tmp = xe.xkey.window;
  XUngrabKeyboard(disp, CurrentTime);
  XPutBackEvent(disp, &xe);
  XSelectInput(disp, tmp, KeyPressMask | KeyReleaseMask);
  XGrabKey(disp, iKeyCode, None, tmp, TRUE, GrabModeAsync, GrabModeAsync);
  XNextEvent(disp, &xe);
  XAllowEvents(disp, ReplayKeyboard, iKeyTime);

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

