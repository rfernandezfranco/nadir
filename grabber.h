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
 
#ifndef GRABBERCLASS_H
#define GRABBERCLASS_H

#include <stdio.h>
#include <stdlib.h>
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
    bool start();
    void stop();
    void flush();
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
};

#endif // GRABBERCLASS_H

