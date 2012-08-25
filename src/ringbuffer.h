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
 
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QMutex>
#include <QObject>
#include <QVector>
#include <QWaitCondition>
#include <limits.h>

#if QT_VERSION >= 0x040400
#include <QAtomicInt>
#else
#define QAtomicInt int
#endif

/**
 * Thread-safe ring buffer - one thread produces and another consumes.
 * A third thread reads maximum
 */
class RingBuffer : public QObject
{
  private:
    unsigned char *ringBuf;
    unsigned long ringBufSize;
    unsigned long fillRateMax;
    unsigned long writeOfs;
    unsigned long readOfs;
    QAtomicInt fillRate;

    int frameCounter;
    QMutex waitForDataMutex;
    QWaitCondition waitForData;

    int channels, sampleSize;
    QVector<QAtomicInt> max;
    void updateMax(long startpos, long endpos);

  public:
    RingBuffer(long size, int channelCount, bool enable32bit);
    ~RingBuffer();
    void reset();
    long readMax(int channelIndex);
    unsigned long getFillRate() { return fillRate; }
    unsigned long getFillRateMax() { return fillRateMax; }

    unsigned char* beginWrite(long& curPos, long& size);
    void endWrite(long curPos);
    unsigned char* beginRead(long& curPos, long& size, bool waitIfEmpty);
    void endRead(long curPos);
    void discardBuffer();
};

#endif

