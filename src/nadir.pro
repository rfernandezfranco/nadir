TEMPLATE = app
TARGET = nadir
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -lX11 -lXtst -L/usr/X11R6/lib -ljack -lasound -lpthread

# Input
HEADERS += mainWidget.h confWidget.h scanLine.h grabber.h \
           capture.h config.h jackcapture.h microphone.h meter.h \
           ringbuffer.h settings.h

FORMS += confWidget.ui mainWidget.ui

SOURCES += main.cpp mainWidget.cpp confWidget.cpp scanLine.cpp grabber.cpp \
           capture.cpp jackcapture.cpp microphone.cpp meter.cpp \
           ringbuffer.cpp settings.cpp

RESOURCES += nadir.qrc

TRANSLATIONS = i18n/es.ts \
               i18n/en.ts
