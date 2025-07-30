#ifndef MOUSE_H
#define MOUSE_H

#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "fixx11h.h"
#include <QCoreApplication>
#include <QSettings>
#include "common.h"

// Simple helper to monitor mouse buttons using X11
class Mouse
{
public:
    Mouse();
    ~Mouse();

    // Open the X11 display and grab the configured button
    bool start();
    // Release the grab and close the display
    void stop();

    // Check for a press of the selected button
    unsigned int grabButtonEvent();
    // Change the button number while running
    void setButtonCode(int i);
    // Read the button from persistent settings
    void loadButtonCode();
    // Returns the number of available mouse buttons
    int getButtonCount() const;

private:
    Display *display;      // connection to the X server
    int screenNumber;      // default screen index
    bool grabbed;          // whether the button is currently grabbed
    int buttonCode;        // X11 button number to watch
};

#endif // MOUSE_H
