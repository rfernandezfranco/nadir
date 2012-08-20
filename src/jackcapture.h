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
 
#ifndef JACKCAPTURE_H
#define JACKCAPTURE_H

#include <qobject.h>
#include <qvector.h>
#include <jack/jack.h>
#include "settings.h"
#include "ringbuffer.h"

class JackCapture : public QObject
{

  private:
    static int capture_callback(jack_nframes_t nframes, void *arg);
    int jack_capture(jack_nframes_t nframes);

    int samplesize, framesize, channels;
    int rate;
    bool jackRunning;

    RingBuffer* ringBuffer;

    jack_client_t *jack_handle;
    QVector<jack_port_t*> jack_in;
    jack_default_audio_sample_t **jackdata;

  public:
    JackCapture(SettingsData *settings);
    ~JackCapture();

    bool isRunning() { return jackRunning; }
    int getRate() { return rate; }
    int initJack();
    int activateJack(RingBuffer* r);
    int deactivateJack(); 
};


#endif
