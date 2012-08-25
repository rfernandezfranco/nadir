#include "key.h"

Key::Key(QObject *parent) :
    QObject(parent)
{
}

void Key::getSym(int keycode, QString *keysym)
{
    QString s;

    switch(keycode){
        case 9:   s = "ESCAPE"; break;
        case 22:  s = QString::fromUtf8("ATRÁS"); break;
        case 36:  s = "INTRO"; break;
        case 37:  s = "CONTROL IZQUIERDA"; break;
        case 50:  s = "SHIFT IZQUIERDA"; break;
        case 62:  s = "SHIFT DERECHA"; break;
        case 64:  s = "ALT IZQUIERDA"; break;
        case 65:  s = "ESPACIO"; break;
        case 66:  s = QString::fromUtf8("BLOQUEO MAYÚSCULAS"); break;
        case 67:  s = "F1"; break;
        case 68:  s = "F2"; break;
        case 69:  s = "F3"; break;
        case 70:  s = "F4"; break;
        case 71:  s = "F5"; break;
        case 72:  s = "F6"; break;
        case 73:  s = "F7"; break;
        case 74:  s = "F8"; break;
        case 75:  s = "F9"; break;
        case 76:  s = "F10"; break;
        case 95:  s = "F11"; break;
        case 96:  s = "F12"; break;
        case 105: s = "CONTROL DERECHA"; break;
        case 107: s = "IMPRIMIR PANTALLA"; break;
        case 108: s = "ALT GR"; break;
        case 110: s = "INICIO"; break;
        case 111: s = "ARRIBA"; break;
        case 112: s = QString::fromUtf8("PÁGINA ANTERIOR"); break;
        case 113: s = "IZQUIERDA"; break;
        case 114: s = "DERECHA"; break;
        case 115: s = "FIN"; break;
        case 116: s = "ABAJO"; break;
        case 117: s = QString::fromUtf8("PÁGINA SIGUIENTE"); break;
        case 118: s = "INSERTAR"; break;
        case 119: s = "SUPRIMIR"; break;
        case 135: s = "MENU"; break;
        case 165: s = "SUPER"; break;

        default: return;
    }

    *keysym = s;
}
