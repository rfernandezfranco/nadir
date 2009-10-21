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
 
#ifndef VIRTUALMOUSECLASS_H
#define VIRTUALMOUSECLASS_H

#include <stdio.h>

class VirtualMouse
{
  public:
    VirtualMouse( Display *d );
    ~VirtualMouse();
    void click();
    void move( int x, int y );

  private:
    Display *dpy;
};

#endif // VIRTUALMOUSECLASS_H

