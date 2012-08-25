#ifndef KEY_H
#define KEY_H

#include <QObject>

class Key : public QObject
{
  Q_OBJECT

  public:
    explicit Key(QObject *parent = 0);
    static void getSym(int keycode, QString *keysym);
};

#endif // KEY_H
