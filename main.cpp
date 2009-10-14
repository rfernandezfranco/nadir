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
 
#include <QApplication>
//#include "settings.h"
#include "mainWidget.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>

using namespace std;

/*
typedef unsigned size_t;

static void
print_key_event (XEvent *ev)
{
   XKeyEvent *key_ev;
   char buffer[20];
   int bufsize = 19;
   KeySym key;
   XComposeStatus compose;
   int char_count;
   string m;

   key_ev = (XKeyEvent *)ev;

   printf("-- INI ----------------------------------------------------------------------------\n");
   printf ("        State: %x KeyCode: %x\n", key_ev->state & ShiftMask, key_ev->keycode);
     char_count = XLookupString(key_ev, buffer, bufsize, &key, &compose);
   buffer[char_count] = '\0';
   printf ("        Char Count: %d KeySym: %x char: |%c|\n", char_count, key, buffer[0]);
   printf("-- FIN ----------------------------------------------------------------------------\n");

   //m = "xmessage -center -buttons Aceptar -geometry 150x60 " +  buffer;
   //m += '\0';

   //system( m.c_str() );
   //cout << m << endl;

   //printf( "\n-- %c --\n", buffer[0] );



   //system( "xmessage -center -buttons Aceptar -geometry 150x60 %c &", buffer[0] );
}
*/

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWidget win;
  win.show();
/*
   Display *dpy;
   int major, minor;
   XEvent  event;
   XClientMessageEvent *xcme;
   int count = 0;
   long delay = 0;
   int ret;
   int quit = 0;
   XKeyEvent *ke;

   if (argc > 2) {
     printf ("Usage: xeviedemo delay (in milliseconds)\n");
     exit(1);
   } else if (argc == 2) {
     delay = strtol(argv[1], 0, 0);
   }
   printf("Delay is %d milliseconds\n", delay);
   delay *= 1000;

   dpy = XOpenDisplay(NULL);
   XevieQueryVersion(dpy, &major, &minor);
   printf("major = %d, minor = %d\n", major, minor);
   if(XevieStart(dpy))
     printf("XevieStart(dpy) finished \n");
   else {
     printf("XevieStart(dpy) failed, only one client is allowed to do event interception\n");
     exit(1);
   }

   XevieSelectInput(dpy, KeyPressMask );

   while( quit == 0 ) {
     XNextEvent(dpy, &event);
     xcme = (XClientMessageEvent *)&event;
     // for readOnly users, send events back to Xserver immediately
     printf("(%4d)", count++);
     switch(event.type) {
       case KeyPress:
            usleep(delay);
            printf(" KeyPress\n");
            print_key_event (&event);
            ke = (XKeyEvent *)&event;
            if( ke->keycode == 9 )
              quit = 1;
            break;
       case KeyRelease: printf(" KeyRelease\n"); break;
       case ButtonPress: usleep(delay); printf(" ButtonPress\n");
            print_key_event (&event);
            break;
       case ButtonRelease: printf(" ButtonRelease\n"); break;
       case MotionNotify: printf(" MotionNotify\n"); break;
       case ClientMessage: printf("ClientMessage: <%s>\n", &xcme->data.b[1]); break;
       default: printf(" unknown event %x\n", event.type); break;
     }
     XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
     if(count > 10000) {
       break;
     }
   }
   XevieEnd(dpy);
   printf("XevieEnd(dpy) finished \n");
*/

  return app.exec();
}

