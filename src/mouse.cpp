#include "mouse.h"

// Basic wrapper around Xlib button grabbing used for mouse-based scan mode

Mouse::Mouse()
{
    grabbed = false;
    loadButtonCode();
}

void Mouse::loadButtonCode()
{
    QCoreApplication::setOrganizationName( ORGANIZATION_NAME );
    QCoreApplication::setOrganizationDomain( ORGANIZATION_DOMAIN );
    QCoreApplication::setApplicationName( APPLICATION_NAME );

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
    if(grabbed && buttonCode > 0){
        XUngrabButton(display, buttonCode, AnyModifier,
                      DefaultRootWindow(display));
        // Release any active pointer grab so synthetic events reach
        // the target window even if the physical button is still pressed
        XUngrabPointer(display, CurrentTime);
        XSync(display, False);
    }
    if(display)
        XCloseDisplay(display);
}

unsigned int Mouse::grabButtonEvent()
{
    unsigned int result = 0;
    while (XPending(display)) {
        XEvent ev;
        XNextEvent(display, &ev);
        if (ev.type == ButtonPress && ev.xbutton.button == buttonCode)
            result = 1;
    }
    return result;
}

void Mouse::setButtonCode(int i)
{
    if(grabbed && buttonCode > 0 && display){
        XUngrabButton(display, buttonCode, AnyModifier,
                      DefaultRootWindow(display));
        XUngrabPointer(display, CurrentTime);
        XSync(display, False);
        grabbed = false;
    }
    buttonCode = i;
    if(buttonCode > 0 && display){
        XGrabButton(display, buttonCode, AnyModifier,
                    DefaultRootWindow(display), False,
                    ButtonPressMask, GrabModeAsync, GrabModeAsync,
                    None, None);
        XSync(display, False);
        grabbed = true;
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

