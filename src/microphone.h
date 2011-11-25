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
#include <QSocketNotifier>

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


class Microphone : public QWidget 
{
  Q_OBJECT

  public:
    Microphone(SettingsData* p_settings, QWidget *parent);
    ~Microphone();
    void init();
    int open_seq(snd_seq_t **seq_handle, int in_ports[],
            int out_ports[], int num_in, int num_out);
    bool isCapturing();
 
  public slots: 
    void stop();
    void capture(bool on);
    void getDb(double d);

  signals:
    void doEvent(double);

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
    bool capturing;
    bool waiting;
    double threshold;
    QTimer *waitTimer;
};
  
#endif // MICROPHONE_H
