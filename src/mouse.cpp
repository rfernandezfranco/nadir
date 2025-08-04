#include "mouse.h"

// Basic wrapper around Xlib button grabbing used for mouse-based scan mode

Mouse::Mouse()
{
    grabbed = false;
    lastDown = false;
    loadButtonCode();
}

void Mouse::loadButtonCode()
{
    QSettings settings;
    settings.beginGroup("Main");
    buttonCode = settings.value("mouseButton", 1).toInt();
    settings.endGroup();
}

bool Mouse::start()
{
    display = XOpenDisplay(nullptr);
    if (!display)
        return false;
    screenNumber = DefaultScreen(display);
    grabbed = false;
    // Button grabs are now performed explicitly via setButtonCode()
    // once the application knows which scan mode is active.
    return true;
}

void Mouse::stop()
{
    ungrabButton();
    if(display)
        XCloseDisplay(display);
}

bool Mouse::grabButtonEvent()
{
    bool triggered = false;

    while (XPending(display)) {
        XEvent ev;
        XNextEvent(display, &ev);
        if (ev.type == ButtonPress && ev.xbutton.button == buttonCode) {
            triggered = !lastDown;
            lastDown = true;
        } else if (ev.type == ButtonRelease && ev.xbutton.button == buttonCode) {
            lastDown = false;
        }
    }

    if (!triggered && display && buttonCode > 0) {
        Window root_return, child_return;
        int root_x, root_y, win_x, win_y;
        unsigned int mask_return;
        XQueryPointer(display, DefaultRootWindow(display),
                      &root_return, &child_return,
                      &root_x, &root_y, &win_x, &win_y,
                      &mask_return);
        bool down = mask_return & buttonMask();
        triggered = down && !lastDown;
        lastDown = down;
    }

    return triggered;
}

void Mouse::ungrabButton()
{
    if(grabbed && buttonCode > 0 && display){
        XUngrabButton(display, buttonCode, AnyModifier,
                      DefaultRootWindow(display));
        // Release any active pointer grab so synthetic events reach
        // the target window even if the physical button is still pressed
        XUngrabPointer(display, CurrentTime);
        XSync(display, False);
        grabbed = false;
    }
}

void Mouse::setButtonCode(int i)
{
    if(buttonCode == i)
        return;

    ungrabButton();
    buttonCode = i;
    lastDown = false;
    if(buttonCode > 0 && display){
        XGrabButton(display, buttonCode, AnyModifier,
                    DefaultRootWindow(display), False,
                    ButtonPressMask, GrabModeAsync, GrabModeAsync,
                    None, None);
        XSync(display, False);
        grabbed = true;
    }
}

unsigned int Mouse::buttonMask() const
{
    switch(buttonCode){
        case 1: return Button1Mask;
        case 2: return Button2Mask;
        case 3: return Button3Mask;
        case 4: return Button4Mask;
        case 5: return Button5Mask;
        default: return 0;
    }
}

int Mouse::getButtonCount() const
{
    unsigned char map[256];
    return XGetPointerMapping(display, map, sizeof(map));
}

Mouse::~Mouse()
{
    stop();
}

