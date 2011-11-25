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
 
#include "jackcapture.h"
#include "config.h"


JackCapture::JackCapture(SettingsData *settings)
{
    channels = settings->getChannels();
    samplesize = settings->getSampleSize();
    framesize = channels * samplesize;
}

JackCapture::~JackCapture()
{
    if (jackRunning) {
        deactivateJack();
    }
    if (jack_handle != 0) {
        jack_client_close(jack_handle);
        jack_handle = 0; 
    }
}


int JackCapture::initJack()
{
    int l1;

    if ((jack_handle = jack_client_open(PACKAGE, JackNullOption, NULL)) == 0) {
        qCritical("jack server not running?\n");
        return 1;
    }

    jack_set_process_callback(jack_handle, capture_callback, (void *)this);
    rate = jack_get_sample_rate(jack_handle);
    for (l1 = 0; l1 < channels; l1++) {
        jack_in.append(jack_port_register(jack_handle,
            QString("in_%1").arg(l1).toAscii().constData(),
            JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
    }
    return(0);
}
 
int JackCapture::activateJack(RingBuffer* buf) 
{
    jackRunning = true;
    if (jack_activate(jack_handle)) {
        qWarning("cannot activate client");
    }
    ringBuffer = buf;
    ringBuffer->reset();
    return(0);
}

int JackCapture::deactivateJack() 
{
    if (jackRunning) {
        jackRunning = false;
        if (jack_deactivate(jack_handle)) {
            qWarning("cannot deactivate client");
        }
    }
    return(0);
}

int JackCapture::capture_callback(jack_nframes_t nframes, void *arg)
{
    JackCapture *rd;

    rd = (JackCapture *)arg;
    return(rd->jack_capture(nframes));
}

int JackCapture::jack_capture(jack_nframes_t nframes)
{
    int l1, i;
    jack_nframes_t l2;
    jack_default_audio_sample_t *jackdata[channels];
    short s;

    unsigned char* ringBuf;
    long writeOfs, ringBufSize;

    for (l1 = 0; l1 < channels; l1++) {  
        jackdata[l1] = (jack_default_audio_sample_t *)
            jack_port_get_buffer(jack_in[l1], nframes);
    }

    ringBuf = ringBuffer->beginWrite(writeOfs, ringBufSize);

    if (samplesize == 2) {
        for (l2 = 0; l2 < nframes; l2++) { 
            for (l1 = 0; l1 < channels; l1++) {
                s = (short)(32767.0 * jackdata[l1][l2]);
                ringBuf[writeOfs++] = (unsigned char)s;
                ringBuf[writeOfs++] = s >> 8;
                if (writeOfs >= ringBufSize) writeOfs -= ringBufSize;
            }
        }
    } else {
        for (l2 = 0; l2 < nframes; l2++) {
            for (l1 = 0; l1 < channels; l1++) {
                i = (int)(2147483647.0 * jackdata[l1][l2]);
                ringBuf[writeOfs++] = (unsigned char)i;  
                ringBuf[writeOfs++] = i >> 8;
                ringBuf[writeOfs++] = i >> 16;
                ringBuf[writeOfs++] = i >> 24;
                if (writeOfs >= ringBufSize) writeOfs -= ringBufSize;
            }
        }
    }
    ringBuffer->endWrite(writeOfs);
    return(0);
}

