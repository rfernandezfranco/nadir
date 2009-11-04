#include <math.h>
#include "meter.h"

#include <iostream>

using namespace std;

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
    timer = new QTimer(this);
    wait = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMeter()));
    connect(this, SIGNAL(dBUpdated(double)), parent, SLOT(getDb(double)));
}

Meter::~Meter()
{
}

void Meter::setDb()
{
    max = curMax;
    if (max < 0) {
        return;
    }
 
    meter_over = (sampleSize == 2) ? METER_OVER : METER_OVER32;

    if (max > 0) {
        _dB = 20.0 * log((double)max/meter_over)/log(10.0);  
        if (_dB < minDB)
            _dB = minDB;  
    }

    emit dBUpdated( _dB );
}

double Meter::dB()
{
  return _dB;
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
    setDb();
}

void Meter::start()
{
    timer->start( CAPTURE_TIME );
}

void Meter::stop()
{
    timer->stop();
}

void Meter::resetGlobalMax()
{
    globalMaxResetCount = 0;
    globalMax = 0;
}
