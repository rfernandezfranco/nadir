#include <alsa/asoundlib.h>
#include <QBoxLayout>
#include <QDir>
#include <QGroupBox>
#include <QFileDialog>
#include <QPushButton>
#include <QSocketNotifier>
#include <QMenuBar>   
#include <QMenu>


#include "microphone.h"
#include "meter.h"

Microphone::Microphone(SettingsData* p_settings)
{
    int l1;
    QString meterText;

    jackCapture = NULL;
    alsaCapture = NULL;
    ringBuffer = NULL;

    settings = p_settings;
    jackMode = settings->getEnableJack();
    open_seq(&seq_handle, in_ports, out_ports, 1, 0);

    ringBuffer = new RingBuffer(settings->getRingBufSize(),
        settings->getChannels(), settings->getEnable32bit());

    if (jackMode) {
        jackCapture = new JackCapture(settings);
        jackCapture->setParent(this);
        jackCapture->initJack();
        settings->setRate(jackCapture->getRate());
    }

    QHBoxLayout *timeBoxLayout = new QHBoxLayout;
    timeBoxLayout->setMargin(3);
    timeBoxLayout->setSpacing(50);

    if (!jackMode) {
        captureToggle = new QCheckBox(this);
        captureToggle->setText(tr("&Capture"));
        QObject::connect(captureToggle, SIGNAL(toggled(bool)),
                this, SLOT(captureToggled(bool)));
        timeBoxLayout->addWidget(captureToggle);
    } else {
        jackCapture->activateJack(ringBuffer);
    }

    for (l1 = 0; l1 < settings->getChannels(); l1++) {
        meters.append(new Meter((tickType) (l1 & 1), ringBuffer, l1,
            settings->getSampleSize(), -settings->getMeterRange(), this));
    }

    QVBoxLayout *guiBoxLayout = new QVBoxLayout;

    guiBoxLayout->addLayout(timeBoxLayout);
    QWidget *guiBox = new QWidget;
    guiBox->setLayout(guiBoxLayout);
	
    setCentralWidget(guiBox);

    if (!jackMode) {
        alsaCapture = new Capture(settings, ringBuffer);
        QObject::connect(alsaCapture, SIGNAL(finished()), this,
                SLOT(stop()));
    }

    show();
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

    err = snd_seq_set_client_name(*seq_handle, PACKAGE);
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

void Microphone::captureToggled(bool on)
{
    int l1;

    if (on) {
        if (!jackMode) {
            if (!alsaCapture->isRunning()) {
                 alsaCapture->start();
            }
        }
    } else {
        if (!jackMode) {
            alsaCapture->stop();
        }
        ringBuffer->reset();
        for (l1 = 0; l1 < meters.size(); l1++) {
            meters[l1]->resetGlobalMax();
        }
    }
}

bool Microphone::grabEvent()
{
  return false;
}

void Microphone::stop()
{
    if (captureToggle->isChecked()) {
        qWarning("ALSA capture failed!");
    }
    captureToggle->setChecked(false);
}
