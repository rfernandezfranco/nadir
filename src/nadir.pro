TEMPLATE = app
TARGET = nadir
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -lX11 -lXtst -L/usr/X11R6/lib -ljack -lasound -lpthread

HEADERS += capture.h \
           config.h \
           confWidget.h \
           jackcapture.h \
           key.h \
           keyboard.h \
           mainWidget.h \
           meter.h \
           microphone.h \
           ringbuffer.h \
           scanLine.h \
           settings.h \
    common.h

FORMS += confWidget.ui \
         mainWidget.ui

SOURCES += capture.cpp \
           confWidget.cpp \
           jackcapture.cpp \
           key.cpp \
           keyboard.cpp \
           main.cpp \
           mainWidget.cpp \
           meter.cpp \
           microphone.cpp \
           ringbuffer.cpp \
           scanLine.cpp \
           settings.cpp

RESOURCES += nadir.qrc

TRANSLATIONS = translations/nadir_es.ts \
               translations/nadir_en.ts

include(translations/translations.pri)

