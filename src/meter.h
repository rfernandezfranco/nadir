/*
 * Copyright (C) 2009-2011 Juan Roldan Ruiz <juan.roldan@gmail.com>
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
 
#ifndef METER_H
#define METER_H

#include <qwidget.h>
#include <qstring.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qsizepolicy.h>
#include <qsize.h>

#include "ringbuffer.h"

#define METER_MINIMUM_WIDTH        300
#define METER_MINIMUM_HEIGHT        45
#define METER_OVER               32000
#define METER_OVER32        2097152000
#define METER_OVER_WIDTH            30
#define METER_TICK_WIDTH            25

#define CAPTURE_TIME               150

enum tickType { TICK_UP, TICK_DOWN };

class Meter : public QWidget
{
  Q_OBJECT

  public:
    Meter(tickType p_tick, RingBuffer *p_ringbuffer, int channelIndex,
      int p_sampleSize, int p_minDB, QWidget* parent=0);
    ~Meter();
    double dB();
    void start();
    void stop();

  public slots: 
    void updateMeter();
    void resetGlobalMax();

  signals:
    void dBUpdated(double);

  protected:
    void setDb();

  private:
    RingBuffer *ringBuffer;
    int channel;
    int sampleSize;
    int minDB;
    int max;
    double _dB;
    double meter_over;
    int curMax, globalMax, globalMaxResetCount;
    QTimer *timer;
    bool wait;
    tickType tick;
};
  
#endif
