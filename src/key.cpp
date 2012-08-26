#include "key.h"

Key::Key(QObject *parent) :
    QObject(parent)
{
}

void Key::getSym(int keycode, QString *keysym)
{
    QString s;

    switch(keycode){
        case 9:   s = trUtf8("ESCAPE"); break;
        case 22:  s = trUtf8("BACKSPACE"); break;
        case 36:  s = trUtf8("ENTER"); break;
        case 37:  s = trUtf8("CONTROL LEFT"); break;
        case 50:  s = trUtf8("SHIFT LEFT"); break;
        case 62:  s = trUtf8("SHIFT RIGHT"); break;
        case 64:  s = trUtf8("ALT LEFT"); break;
        case 65:  s = trUtf8("SPACE"); break;
        case 66:  s = trUtf8("CAPS LOCK"); break;
        case 67:  s = trUtf8("F1"); break;
        case 68:  s = trUtf8("F2"); break;
        case 69:  s = trUtf8("F3"); break;
        case 70:  s = trUtf8("F4"); break;
        case 71:  s = trUtf8("F5"); break;
        case 72:  s = trUtf8("F6"); break;
        case 73:  s = trUtf8("F7"); break;
        case 74:  s = trUtf8("F8"); break;
        case 75:  s = trUtf8("F9"); break;
        case 76:  s = trUtf8("F10"); break;
        case 95:  s = trUtf8("F11"); break;
        case 96:  s = trUtf8("F12"); break;
        case 105: s = trUtf8("CONTROL RIGHT"); break;
        case 107: s = trUtf8("PRINT SCREEN"); break;
        case 108: s = trUtf8("ALT GR"); break;
        case 110: s = trUtf8("HOME"); break;
        case 111: s = trUtf8("UP"); break;
        case 112: s = trUtf8("PAGE UP"); break;
        case 113: s = trUtf8("LEFT"); break;
        case 114: s = trUtf8("RIGHT"); break;
        case 115: s = trUtf8("END"); break;
        case 116: s = trUtf8("DOWN"); break;
        case 117: s = trUtf8("PAGE DOWN"); break;
        case 118: s = trUtf8("INSERT"); break;
        case 119: s = trUtf8("DELETE"); break;
        case 135: s = trUtf8("MENU"); break;
        case 165: s = trUtf8("SUPER"); break;

        default: return;
    }

    *keysym = s;
}
