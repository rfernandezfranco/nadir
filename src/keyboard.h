#ifndef KEYBOARD_H
#define KEYBOARD_H

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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/extensions/XTest.h>
#include <X11/Intrinsic.h>
#include <X11/keysymdef.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// This header solves a X11-Qt header issue
#include <fixx11h.h>

#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "common.h"

class Keyboard
{
  public:
    Keyboard();
    ~Keyboard();
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
    void loadKeyCode();
    void setKeyCode(int i);
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
    int keyCode;

    int x11_fd;
    fd_set in_fds;
    struct timeval tv;
    bool grabbed;

    int i;
    char *char_ptr, buf1[32], buf2[32], *keys, *saved;
    int PrintUp;
};

#endif // KEYBOARD_H
