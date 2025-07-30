#include "mouse.h"

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
    disp = XOpenDisplay(nullptr);
    if (!disp)
        return false;
    screen = DefaultScreen(disp);
    grabbed = false;
    if(buttonCode > 0){
        XGrabButton(disp, buttonCode, AnyModifier,
                    DefaultRootWindow(disp), False,
                    ButtonPressMask, GrabModeAsync, GrabModeAsync,
                    None, None);
        grabbed = true;
    }
    return true;
}

void Mouse::stop()
{
    if(grabbed && buttonCode > 0)
        XUngrabButton(disp, buttonCode, AnyModifier, DefaultRootWindow(disp));
    if(disp)
        XCloseDisplay(disp);
}

unsigned int Mouse::grabButtonEvent()
{
    unsigned int event = 0;
    while(XPending(disp)){
        XEvent ev;
        XNextEvent(disp, &ev);
        if(ev.type == ButtonPress && ev.xbutton.button == buttonCode)
            event = 1;
    }
    return event;
}

void Mouse::setButtonCode(int i)
{
    if(grabbed && buttonCode > 0 && disp){
        XUngrabButton(disp, buttonCode, AnyModifier, DefaultRootWindow(disp));
        grabbed = false;
    }
    buttonCode = i;
    if(buttonCode > 0 && disp){
        XGrabButton(disp, buttonCode, AnyModifier,
                    DefaultRootWindow(disp), False,
                    ButtonPressMask, GrabModeAsync, GrabModeAsync,
                    None, None);
        grabbed = true;
    }
}

int Mouse::getButtonCount() const
{
    unsigned char map[256];
    return XGetPointerMapping(disp, map, sizeof(map));
}

Mouse::~Mouse()
{
}

