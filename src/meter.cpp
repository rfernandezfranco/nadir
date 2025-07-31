/*
 * Copyright (C) 2009-2012 Juan Roldan Ruiz <juan.roldan@gmail.com>
 *  
 * This file is part of Nadir.
 * 
 * Nadir is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nadir is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nadir.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <math.h>
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
    timer = new QTimer(this);
    wait = false;
    _dB = p_minDB; // initialize with minimum value so the meter starts at zero
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
    } else {
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
