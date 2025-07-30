#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>

/// Delay used for simulated key presses (milliseconds)
constexpr int KEY_PRESS_TIME = 250;
/// Delay used for simulated double click (milliseconds)
constexpr int KEY_PRESS_TIME2 = 100;

inline bool bit(const char *c, int x) { return c[x / 8] & (1 << (x % 8)); }

constexpr int KEYSYM_STRLEN = 64;

// Modifier return values
constexpr int SHIFT_DOWN = 1;
constexpr int LOCK_DOWN  = 5;
constexpr int CONTROL_DOWN = 3;
constexpr int ISO3_DOWN = 4;
constexpr int MODE_DOWN = 5;

// Indexes for XKeycodeToKeysym()
constexpr int SHIFT_INDEX = 1;      ///< shifted keys
constexpr int MODE_INDEX = 2;
constexpr int MODESHIFT_INDEX = 3;
constexpr int ISO3_INDEX = 4;        ///< TODO does not work?
constexpr int ISO3SHIFT_INDEX = 4;

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
#include "fixx11h.h"

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
    unsigned int grabKeyEvent();
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
    const char *KeyCodeToStr(int code, int down, int mod);
    int KeyModifiers(char *keys);

  private:
    Display *disp;
    int screen;
    int escapeCode;
    int keyCode;

    // key state buffers used by grabKeyEvent
    char buf1[32], buf2[32];
    char *keys{buf1};
    char *saved{buf2};

    bool printUp;
};

#endif // KEYBOARD_H
