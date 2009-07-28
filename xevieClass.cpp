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
 
#include "xevieClass.h"

XevieClass::XevieClass()
{
  //_delay = strtol( delay, 0, 0 );
  _delay = 1;
}

bool XevieClass::start()
{
  dpy = XOpenDisplay(NULL);
  x11_fd = ConnectionNumber(dpy);
  scr = DefaultScreen( dpy );
  //Window root, child;

  //if ( XQueryPointer( dpy, DefaultRootWindow( dpy ),
  //       &root, &child, &x, &y, &wx, &wy, &m ) );


  XevieQueryVersion(dpy, &major, &minor);
  //printf("major = %d, minor = %d\n", major, minor);

  if(!XevieStart(dpy))
    return false;

  return true;
}

bool XevieClass::grabEvent()
{
  grabbed = false;

  //XNextEvent(dpy, &event);
  //xcme = (XClientMessageEvent *)&event;

  // Create a File Description Set containing x11_fd
  FD_ZERO(&in_fds);
  FD_SET(x11_fd, &in_fds);

  // Set timer
  tv.tv_usec = 0;
  tv.tv_sec = 0.0005;

  // Wait for X Event or a Timer
  if (select(x11_fd+1, &in_fds, 0, 0, &tv)){
      //printf("Event Received!\n");
      //click();
      grabbed = true;
  }
  //else
      // Handle timer here
      //printf("Timer Fired!\n");

  // Handle XEvents and flush the input 
  while(XPending(dpy))
      XNextEvent(dpy, &event);

  return grabbed;


/*
  // for readOnly users, send events back to Xserver immediately
  //printf("(%4d)", count++);
  switch(event.type) {
    case KeyPress:
      usleep(_delay);
      printf(" KeyPress\n");
      printKeyEvent (&event);
      ke = (XKeyEvent *)&event;
      break;
    case KeyRelease: printf(" KeyRelease\n"); break;
    case ButtonPress: usleep(_delay); printf(" ButtonPress\n");
                      printKeyEvent (&event);
                      break;
    case ButtonRelease: printf(" ButtonRelease\n"); break;
    case MotionNotify: printf(" MotionNotify\n"); break;
    case ClientMessage: printf("ClientMessage: <%s>\n", &xcme->data.b[1]); break;
    default: printf(" unknown event %x\n", event.type); break;
  }
*/
  //system("xclock &");
  //click();
  //XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
}

void XevieClass::printKeyEvent (XEvent *ev)
{
/*  XKeyEvent *key_ev;
  char buffer[20];
  int bufsize = 19;
  KeySym key;
  XComposeStatus compose;
  int char_count;

  key_ev = (XKeyEvent *)ev;

  printf ("        State: %x KeyCode: %x\n", key_ev->state & ShiftMask, key_ev->keycode);
  char_count = XLookupString(key_ev, buffer, bufsize, &key, &compose);
  buffer[char_count] = '\0';
  printf ("        Char Count: %d KeySym: %x char: |%c|\n", char_count,
      (unsigned int)key, buffer[0]);
  fflush( stdout );
  */
}

void XevieClass::end()
{
  XevieEnd(dpy);
  XCloseDisplay(dpy);
}

void XevieClass::click()
{
  XTestFakeButtonEvent( dpy, 1, True, 0 );
  XTestFakeButtonEvent( dpy, 1, False, 250 );
}

void XevieClass::move( int x, int y )
{
  XTestFakeMotionEvent( dpy, scr, x, y, 10 );
}
