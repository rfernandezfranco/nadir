#ifndef MOUSE_H
#define MOUSE_H

#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "fixx11h.h"
#include <QSettings>
#include "common.h"

// Simple helper to monitor mouse buttons using X11
class Mouse
{
public:
    Mouse();
    ~Mouse();

    // Open the X11 display
    bool start();
    // Release the grab and close the display
    void stop();

    // Check for a press of the selected button
    bool grabButtonEvent();
    // Change the button number while running
    void setButtonCode(int i);
    // Read the button from persistent settings
    void loadButtonCode();
    // Currently active button number
    int getButtonCode() const { return buttonCode; }
    // Returns the number of available mouse buttons
    int getButtonCount() const;
    // Wait until the watched button is released
    void waitForRelease();
    // Release any active pointer grab without altering the button grab
    void ungrabPointer();

private:
    Display *display;      // connection to the X server
    int screenNumber;      // default screen index
    bool grabbed;          // whether the button is currently grabbed
    int buttonCode;        // X11 button number to watch
    bool lastDown;         // last known state of the watched button

    void ungrabButton();   // helper to release current grab
    unsigned int buttonMask() const; // mask for XQueryPointer
};

#endif // MOUSE_H
