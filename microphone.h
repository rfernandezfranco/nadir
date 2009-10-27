#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>
#include <QList>
#include <QVector>
#include <QTimer>
#include <QMainWindow>
#include <QApplication>
#include <alsa/asoundlib.h>

#include "capture.h"
#include "jackcapture.h"
#include "ringbuffer.h"
#include "meter.h"
#include "settings.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
static const char ABOUTMSG[] = APP_NAME " " PACKAGE_VERSION "\n"
#else
static const char ABOUTMSG[] = "Captura\n"
#endif
                          "(C) 2002-2003 Matthias Nagorni (SuSE AG Nuremberg)\n"
			  "(C) 2009 Frank Kober\n"
			  "(C) 2009 David Henningsson\n"
			  "(C) 2009 Guido Scholz\n\n"
                          " is licensed under the GPL.\n";


class Microphone 
{
  public:
    Microphone(SettingsData* p_settings);
    ~Microphone();
    int open_seq(snd_seq_t **seq_handle, int in_ports[],
            int out_ports[], int num_in, int num_out);
    bool grabEvent();
 
  public slots: 
    void stop();
    void captureToggled(bool on);

  private:
    QLabel *currentFileLabel, *timeLabel, *bufLabel, *maxBufLabel, *statusLabel;
    QPushButton *recButton, *pauseButton, *stopButton;
    QCheckBox *captureToggle;
    Capture *alsaCapture;
    JackCapture *jackCapture;
    RingBuffer *ringBuffer;
    SettingsData *settings;
    QVector<Meter*> meters;
    snd_seq_t *seq_handle;
    bool jackMode;
    int in_ports[2], out_ports[2];
};
  
#endif // MICROPHONE_H
