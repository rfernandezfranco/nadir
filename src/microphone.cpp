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
 
#include "microphone.h"

Microphone::Microphone(SettingsData* p_settings, QWidget *parent)
{
    jackCapture = NULL;
    alsaCapture = NULL;
    ringBuffer = NULL;
    settings = p_settings;
    init();

    waitTimer = new QTimer(this);
    waiting = false;
    capturing= false;

    connect(this, SIGNAL(doEvent(double)), parent, SLOT(micEvent(double)));
}

Microphone::~Microphone()
{
    if (jackCapture != NULL) {
        delete jackCapture;
        jackCapture = NULL;
    }

    if (alsaCapture != NULL) {
        delete alsaCapture;
        alsaCapture = NULL;
    }

    if (ringBuffer != NULL) {
        delete ringBuffer;
        ringBuffer = NULL;
    }
}

void Microphone::init()
{
    jackMode = settings->getEnableJack();
    open_seq(&seq_handle, in_ports, out_ports, 1, 0);

    ringBuffer = new RingBuffer(settings->getRingBufSize(),
        settings->getChannels(), settings->getEnable32bit());

    if (jackMode) {
        jackCapture = new JackCapture(settings);
        jackCapture->setParent(this);
        jackCapture->initJack();
        settings->setRate(jackCapture->getRate());
        jackCapture->activateJack(ringBuffer);
    };

    for (int l1 = 0; l1 < settings->getChannels(); l1++) {
        meters.append(new Meter((tickType) (l1 & 1), ringBuffer, l1,
            settings->getSampleSize(), -settings->getMeterRange(), this));
    }

    if (!jackMode) {
        alsaCapture = new Capture(settings, ringBuffer);
        QObject::connect(alsaCapture, SIGNAL(finished()), this,
                SLOT(stop()));
    };
}

int Microphone::open_seq(snd_seq_t **seq_handle, int in_ports[],
        int out_ports[], int num_in, int num_out)
{
    int l1;
    int err;
    char portname[64];

    err = snd_seq_open(seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err < 0) {
        qWarning("Error opening ALSA sequencer (%s).", snd_strerror(err));
        return(-1);
    }

    err = snd_seq_set_client_name(*seq_handle, "nadir");
    if (err < 0) {
        qWarning("Error setting ALSA client name (%s).", snd_strerror(err));
        return(-1);
    }
    
    for (l1 = 0; l1 < num_in; l1++) {
        sprintf(portname, "in_%d", l1);
        if ((in_ports[l1] = snd_seq_create_simple_port(*seq_handle, portname,
                        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                        SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
            qWarning("Error creating sequencer port (%s).",
                    snd_strerror(in_ports[l1]));
            return(-1);
        }
    }  
    for (l1 = 0; l1 < num_out; l1++) {
        sprintf(portname, "out_%d", l1);
        if ((out_ports[l1] = snd_seq_create_simple_port(*seq_handle, portname,
                        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                        SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
            qWarning("Error creating sequencer port (%s).",
                    snd_strerror(out_ports[l1]));
            return(-1);
        }
    }  
    return(0);
}

void Microphone::capture(bool on)
{
    int l1;

    if (on) {
        capturing = true;
        if (!jackMode) {
            if (!alsaCapture->isRunning()) {
                 alsaCapture->start();
            }
        }
        for (l1 = 0; l1 < meters.size(); l1++) {
            meters[l1]->start();
        }
    } else {
        capturing = false;
        if (!jackMode) {
            alsaCapture->stop();
        }
        ringBuffer->reset();
        for (l1 = 0; l1 < meters.size(); l1++) {
            meters[l1]->resetGlobalMax();
            meters[l1]->stop();
        }
    }
}

bool Microphone::isCapturing()
{
  return capturing;
}

void Microphone::getDb(double d)
{
    emit doEvent( d );
}

void Microphone::stop()
{
    if ( capturing ) {
        qWarning("ALSA capture failed!");
    }
    capturing = false;
}
