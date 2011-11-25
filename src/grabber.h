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
 
#ifndef GRABBERCLASS_H
#define GRABBERCLASS_H

#define KEYCODE 73 // F7 key
#define KEYPRESSTIME 250
#define KEYPRESSTIME2 100

#define BIT(c, x)   ( c[x/8]&(1<<(x%8)) )
#define KEYSYM_STRLEN   64
#define SHIFT_DOWN  1
#define LOCK_DOWN   5
#define CONTROL_DOWN    3
#define ISO3_DOWN    4
#define MODE_DOWN    5
/* I think it is pretty standard */
#define SHIFT_INDEX 1  /*index for XKeycodeToKeySym(), for shifted keys*/
#define MODE_INDEX 2
#define MODESHIFT_INDEX 3
#define ISO3_INDEX 4 //TODO geht leider nicht??
#define ISO3SHIFT_INDEX 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>

class Grabber {
  public:
    Grabber();
    ~Grabber();
    void snoop();
    void noSnoop();
    bool start();
    void stop();
    void flush();
    void snoop_all_windows(Window root, unsigned long type);
    char *TranslateKeyCode(XEvent *ev);
    unsigned int grabEvent();
    void move( int x, int y );
    void key( int k );
    void click();
    void doubleClick();
    void rightClick();
    void drag();
    void drop();
    void setEscapeCode( int i );
    Display *getDisplay();

   int StrToChar(char *data);
   char *KeyCodeToStr(int code, int down, int mod);
   int KeyModifiers(char *keys);

  private:
    Display *disp;
    int screen;
    XEvent xe;
    XKeyEvent *ke;
    int iKeyCode;
    int iKeyState;
    int iKeyTime;
    int iKeyType;
    KeySym keysym;
    char *keyString;
    int escapeCode;

    int x11_fd;
    fd_set in_fds;
    struct timeval tv;
    bool grabbed;

    int i;
   char    *char_ptr,
           buf1[32],   buf2[32],
           *keys,
           *saved;
   int PrintUp;
};

#endif // GRABBERCLASS_H

