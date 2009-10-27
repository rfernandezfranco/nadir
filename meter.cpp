#include <qpolygon.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qbrush.h>
#include <qtimer.h>
#include <qsizepolicy.h>
#include <qsize.h>
#include <qboxlayout.h>
#include <math.h>
#include <iostream>

#include "meter.h"


Meter::Meter(tickType p_tick, RingBuffer *p_ringbuffer, int channelIndex,
int p_sampleSize, int p_minDB, QWidget* parent) : QWidget (parent)
{
    tick = p_tick;
    channel = channelIndex;
    minDB = p_minDB;
    ringBuffer = p_ringbuffer;
    sampleSize = p_sampleSize;
    globalMaxResetCount = 0;
    globalMax = 0;
    setPalette(QPalette(QColor(0, 20, 100), QColor(0, 20, 100)));
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateMeter()));
    timer->start(200);
}

Meter::~Meter()
{
}

void Meter::paintEvent(QPaintEvent*)
{
    double dB, meter_over;
    int max;

    max = curMax;
    if (max < 0) {
        return;
    }
 
    meter_over = (sampleSize == 2) ? METER_OVER : METER_OVER32;

    if (max > 0) {
        dB = 20.0 * log((double)max/meter_over)/log(10.0);  
        if (dB < minDB)
            dB = minDB;  

        std::cout << dB << std::endl;
    }
}

void Meter::updateMeter()
{
    if (globalMaxResetCount > 20) {
        globalMaxResetCount = 0;
        globalMax = 0;
    }

    curMax = ringBuffer->readMax(channel);
    if (curMax > globalMax) {
        globalMax = curMax;
        globalMaxResetCount = 0;
    }

    globalMaxResetCount++;

    Meter::update();
    timer->start(200);
}

QSize Meter::sizeHint() const
{
    return QSize(METER_MINIMUM_WIDTH, METER_MINIMUM_HEIGHT); 
}

QSizePolicy Meter::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::MinimumExpanding,
            QSizePolicy::MinimumExpanding);
}

void Meter::resetGlobalMax()
{
    globalMaxResetCount = 0;
    globalMax = 0;
}
