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
