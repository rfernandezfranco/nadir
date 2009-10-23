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
 
#include "virtual.h"

Virtual::Virtual()
{
}

Virtual::Virtual( Display *d )
{
  dpy = d;
  screen = DefaultScreen( dpy );
}

void Virtual::open()
{
  dpy = XOpenDisplay( NULL );
  screen = DefaultScreen( dpy );
}

void Virtual::setDisplay( Display *d )
{
  dpy = d;
}

void Virtual::close()
{
  XCloseDisplay( dpy );
}

void Virtual::move( int x, int y )
{
  XTestFakeMotionEvent( dpy, screen, x, y, 10 );
}

void Virtual::click(void)
{
  XTestFakeButtonEvent( dpy, 1, True, 0 );
  XTestFakeButtonEvent( dpy, 1, False, 250 );
}

void Virtual::doubleClick(void)
{
  XTestFakeButtonEvent( dpy, 1, True, 100 );
  XTestFakeButtonEvent( dpy, 1, False, 100 );
  XTestFakeButtonEvent( dpy, 1, True, 100 );
  XTestFakeButtonEvent( dpy, 1, False, 100 );
}

void Virtual::rightClick(void)
{
  XTestFakeButtonEvent( dpy, 3, True, 0 );
  XTestFakeButtonEvent( dpy, 3, False, 250 );
}

void Virtual::drag(void)
{
  XTestFakeButtonEvent( dpy, 1, True, 0 );
}

void Virtual::drop(void)
{
  XTestFakeButtonEvent( dpy, 1, False, 0 );
}

Virtual::~Virtual()
{
}

