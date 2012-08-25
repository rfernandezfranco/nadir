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
 
#ifndef CAPTURE_H
#define CAPTURE_H

#include <QString>
#include <QThread>
#include <alsa/asoundlib.h>
#include "ringbuffer.h"
#include "settings.h"

class Capture : public QThread
{
  private:
    RingBuffer *ringBuffer;
    unsigned char *periodBuf;
    snd_pcm_t *capture_handle;
    int samplesize, rate, framesize, channels, periodsize, periods;
    bool doCapture;
    QString pcmName;

  private: 
    snd_pcm_t *open_pcm();

  public:
    Capture(SettingsData *settings, RingBuffer *p_ringBuffer);
    ~Capture();
    virtual void run();                    
    void stop();
};
  
#endif
