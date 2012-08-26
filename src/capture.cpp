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
 
#include <sched.h>
#include <sys/mman.h>
#include <qwaitcondition.h>
#include "capture.h"

Capture::Capture(SettingsData *settings, RingBuffer *p_ringBuffer) 
{
    ringBuffer = p_ringBuffer;
    framesize = settings->getFrameSize();
    channels = settings->getChannels();
    samplesize = settings->getSampleSize();
    pcmName = settings->getAlsaPcmName();
    rate = settings->getRate();
    periods = settings->getAlsaPeriods();
    periodsize = settings->getAlsaPeriodsize();
}

Capture::~Capture()
{
    doCapture = false;
    wait();
}

snd_pcm_t *Capture::open_pcm()
{
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_uframes_t buffersize_return;
    unsigned int tmp;
    int err;

    err = snd_pcm_open(&pcm_handle, qPrintable(pcmName),
            SND_PCM_STREAM_CAPTURE, 0); 
    if (err < 0) {
        qWarning("Error opening PCM device %s (%s)", qPrintable(pcmName),
                snd_strerror(err));
        return(NULL);
    }

    snd_pcm_hw_params_alloca(&hwparams);
    err = snd_pcm_hw_params_any(pcm_handle, hwparams);
    if (err < 0) {
        qWarning("Can not configure this PCM device (%s).", snd_strerror(err));
        snd_pcm_close(pcm_handle);
        return(NULL);
    }
    err = snd_pcm_hw_params_set_access(pcm_handle, hwparams,
                SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        qWarning("Error setting access (%s).", snd_strerror(err));
        snd_pcm_close(pcm_handle);
        return(NULL);
    }
    if (samplesize == 2) {
        err = snd_pcm_hw_params_set_format(pcm_handle, hwparams,
                    SND_PCM_FORMAT_S16_LE);
        if (err < 0) {
            snd_pcm_close(pcm_handle);
            qWarning("Error setting format (%s).", snd_strerror(err));
            return(NULL);
        }
    } else {
        err = snd_pcm_hw_params_set_format(pcm_handle, hwparams,
                    SND_PCM_FORMAT_S32_LE);
        if (err < 0) {
            snd_pcm_close(pcm_handle);
            qWarning("Error setting format (%s).", snd_strerror(err));
            return(NULL);
        }
    }
    tmp = (unsigned int)rate;
    err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &tmp, 0);
    if (err < 0) {
        qWarning("Error setting rate (%s).", snd_strerror(err));
        snd_pcm_close(pcm_handle);
        return(NULL);
    }
    err = snd_pcm_hw_params_set_channels(pcm_handle, hwparams, channels);
    if (err < 0) {
        snd_pcm_close(pcm_handle);
        qWarning("Error setting channels (%s).", snd_strerror(err));
        return(NULL);
    }
    err = snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0);
    if (err < 0) {
        snd_pcm_close(pcm_handle);
        qWarning("Error setting periods (%s).", snd_strerror(err));
        return(NULL);
    }
    buffersize_return = periodsize * periods;
    err = snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, 
                    &buffersize_return);
    if (err < 0) {
        qWarning("Error setting buffersize (%s).", snd_strerror(err));
        snd_pcm_close(pcm_handle);
        return(NULL);
    }
    if (buffersize_return != (snd_pcm_uframes_t)(periodsize * periods)) {
        qWarning("Periodsize %d is not available on your hardware. "
            "Using %d instead.",
            periodsize, (int)buffersize_return / periods);
        periodsize = buffersize_return / periods;
    }
    err = snd_pcm_hw_params(pcm_handle, hwparams);
    if (err < 0) {
        qWarning("Error setting HW params (%s).", snd_strerror(err));
        snd_pcm_close(pcm_handle);
        return(NULL);
    }
    //qWarning("Using periodsize %d frames.", periodsize);
    return(pcm_handle);
}


void Capture::run() 
{
    int pcmreturn;

    doCapture = true;

    if (!(capture_handle = open_pcm())) {
        qWarning("Could not open PCM for capture.");
        doCapture = false;
        return;
    }
    periodBuf = (unsigned char *)malloc(periodsize * framesize);
    ringBuffer->reset();

    struct sched_param schp;
    memset(&schp, 0, sizeof(schp));
    schp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &schp) != 0) {
        //qWarning("Can't set sched_setscheduler - using normal priority");
    } else {
        qDebug("Set SCHED_FIFO");
    }

    while(doCapture) {
        while ((pcmreturn = snd_pcm_readi(capture_handle, periodBuf,
                        periodsize)) < 0) {
            snd_pcm_prepare(capture_handle);
            //qWarning("<<<<<<<<<<<<<<< Buffer Overrun >>>>>>>>>>>>>>>");
        }

        long count = periodsize * framesize;
        long size, curPos, blocksize;
        unsigned char* ringBuf;
        ringBuf = ringBuffer->beginWrite(curPos, size);
        blocksize = size - curPos;
        if (blocksize >= count) {
            memcpy(&ringBuf[curPos], periodBuf, count);
        } else {
            // Wrap around ringbuffer
            memcpy(&ringBuf[curPos], periodBuf, blocksize);
            curPos = 0;
            count -= blocksize;
            memcpy(&ringBuf[curPos], &periodBuf[blocksize], count);
        }
        curPos += count;
        ringBuffer->endWrite(curPos);
    }
    snd_pcm_close(capture_handle);
    free(periodBuf);
}

void Capture::stop()
{
    doCapture = false;
}
