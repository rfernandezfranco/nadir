#ifndef MOUSE_H
#define MOUSE_H

#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "fixx11h.h"
#include <QCoreApplication>
#include <QSettings>
#include "common.h"

class Mouse
{
public:
    Mouse();
    ~Mouse();
    bool start();
    void stop();
    unsigned int grabButtonEvent();
    void setButtonCode(int i);
    void loadButtonCode();
    int getButtonCount() const;

private:
    Display *disp;
    int screen;
    bool grabbed;
    int buttonCode;
};

#endif // MOUSE_H
