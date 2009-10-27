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
