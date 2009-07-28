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
 
#ifndef XEVIECLASS_H
#define XEVIECLASS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

class XevieClass
{
  public:
    XevieClass();
    bool start();
    void end();
    bool grabEvent();
    void printKeyEvent (XEvent *ev);
    void click();
    void move( int x, int y );

  //private:
    Display *dpy;
    long _delay;
    int major, minor;
    XEvent  event;
    XClientMessageEvent *xcme;
    XKeyEvent *ke;

    int x11_fd;
    fd_set in_fds;
    struct timeval tv;
    bool grabbed;
    int scr;
};

#endif // XEVIECLASS_H

