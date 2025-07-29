#include "key.h"

Key::Key(QObject *parent) :
    QObject(parent)
{
}

void Key::getSym(int keycode, QString *keysym)
{
    QString s;

    switch(keycode){
        case 9:   s = tr("ESCAPE"); break;
        case 22:  s = tr("BACKSPACE"); break;
        case 36:  s = tr("ENTER"); break;
        case 37:  s = tr("CONTROL LEFT"); break;
        case 50:  s = tr("SHIFT LEFT"); break;
        case 62:  s = tr("SHIFT RIGHT"); break;
        case 64:  s = tr("ALT LEFT"); break;
        case 65:  s = tr("SPACE"); break;
        case 66:  s = tr("CAPS LOCK"); break;
        case 67:  s = tr("F1"); break;
        case 68:  s = tr("F2"); break;
        case 69:  s = tr("F3"); break;
        case 70:  s = tr("F4"); break;
        case 71:  s = tr("F5"); break;
        case 72:  s = tr("F6"); break;
        case 73:  s = tr("F7"); break;
        case 74:  s = tr("F8"); break;
        case 75:  s = tr("F9"); break;
        case 76:  s = tr("F10"); break;
        case 95:  s = tr("F11"); break;
        case 96:  s = tr("F12"); break;
        case 105: s = tr("CONTROL RIGHT"); break;
        case 107: s = tr("PRINT SCREEN"); break;
        case 108: s = tr("ALT GR"); break;
        case 110: s = tr("HOME"); break;
        case 111: s = tr("UP"); break;
        case 112: s = tr("PAGE UP"); break;
        case 113: s = tr("LEFT"); break;
        case 114: s = tr("RIGHT"); break;
        case 115: s = tr("END"); break;
        case 116: s = tr("DOWN"); break;
        case 117: s = tr("PAGE DOWN"); break;
        case 118: s = tr("INSERT"); break;
        case 119: s = tr("DELETE"); break;
        case 135: s = tr("MENU"); break;
        case 165: s = tr("SUPER"); break;

        default: return;
    }

    *keysym = s;
}
