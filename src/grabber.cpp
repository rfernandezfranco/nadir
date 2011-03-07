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

  x11_fd = ConnectionNumber(disp);
  XFlush(disp);
  grabbed = false;
  
  snoop();

  return true;
}

void Grabber::snoop()
{
  snoop_all_windows(DefaultRootWindow(disp), KeyPressMask);
}

void Grabber::noSnoop()
{
  snoop_all_windows(DefaultRootWindow(disp), NoEventMask);
}

void Grabber::flush()
{
  XFlush(disp);
}

Display *Grabber::getDisplay()
{
  return disp;
}

void Grabber::snoop_all_windows(Window root, unsigned long type)
{
  static int level = 0;
  Window parent, *children;
  unsigned int i, nchildren;
  int stat;

  level++;

  stat = XQueryTree(disp, root, &root, &parent, &children, &nchildren);
  if (stat == FALSE)
   {
     fprintf(stderr, "Can't query window tree...\n");
     return;
   }

  if (nchildren == 0)
    return;

  XSelectInput(disp, root, type);

  for(i=0; i < nchildren; i++)
   {
     XSelectInput(disp, children[i], type);
     snoop_all_windows(children[i], type);
   }     

  XFree((char *)children);
}

#define KEY_BUFF_SIZE 256
static char key_buff[KEY_BUFF_SIZE];

char *Grabber::TranslateKeyCode(XEvent *ev)
{
  int count;
  char *tmp;
  KeySym ks;

  if (ev)
   {
     count = XLookupString((XKeyEvent *)ev, key_buff, KEY_BUFF_SIZE, &ks,NULL);
     key_buff[count] = '\0';

     if (count == 0)
      {
        tmp = XKeysymToString(ks);
        if (tmp)
          strcpy(key_buff, tmp);
        else
          strcpy(key_buff, "");
      }

     return key_buff;
   }
  else
    return NULL;
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

  XGrabKey(disp, KEYCODE, KeyPress, DefaultRootWindow(disp), TRUE, GrabModeAsync, GrabModeAsync);

  // Handle XEvents and flush the input 
  while(XPending(disp))
      XNextEvent(disp, &xe);

  if( grabbed ){
    iKeyCode = xe.xkey.keycode;
    iKeyState = xe.xkey.state;
    iKeyTime = xe.xkey.time;
    iKeyType = xe.type;
 
#ifdef DEBUG
      switch (iKeyType) {
        case KeyPress:
          fprintf(stderr, "Key pressed: ");
          break;
        case KeyRelease:
          fprintf(stderr, "key released: ");
          break;
      };
#endif

    keysym = XKeycodeToKeysym(disp, iKeyCode, 0);
    keyString = XKeysymToString(keysym);
    fprintf(stderr, "%s - %i\n", keyString, iKeyCode );
    
    // Return 2 when pressing escape key
    if( iKeyCode == escapeCode ){
      return 2;
    };
    snoop();
  };

  // Return 1 when pressing any key but escape key
  if( grabbed && iKeyType == KeyPress)
    return 1;
  // Return 0 if no key has been pressed
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

void Grabber::key( int k )
{
  XTestFakeKeyEvent( disp, k, True, 0 );
  XTestFakeKeyEvent( disp, k, False, KEYPRESSTIME );
}

void Grabber::click(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME );
}

void Grabber::doubleClick(void)
{
  XTestFakeButtonEvent( disp, 1, True, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, True, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME2 );
}

void Grabber::rightClick(void)
{
  XTestFakeButtonEvent( disp, 3, True, 0 );
  XTestFakeButtonEvent( disp, 3, False, KEYPRESSTIME );
}

void Grabber::drag(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
}

void Grabber::drop(void)
{
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME );
}

Grabber::~Grabber()
{
}

