#ifndef METER_H
#define METER_H

#include <qwidget.h>
#include <qstring.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qsizepolicy.h>
#include <qsize.h>

#include "ringbuffer.h"

#define METER_MINIMUM_WIDTH        300
#define METER_MINIMUM_HEIGHT        45
#define METER_OVER               32000
#define METER_OVER32        2097152000
#define METER_OVER_WIDTH            30
#define METER_TICK_WIDTH            25

#define CAPTURE_TIME               150

enum tickType { TICK_UP, TICK_DOWN };

class Meter : public QWidget
{
  Q_OBJECT

  public:
    Meter(tickType p_tick, RingBuffer *p_ringbuffer, int channelIndex,
      int p_sampleSize, int p_minDB, QWidget* parent=0);
    ~Meter();
    double dB();
    void start();
    void stop();

  public slots: 
    void updateMeter();
    void resetGlobalMax();

  signals:
    void dBUpdated(double);

  protected:
    void setDb();

  private:
    RingBuffer *ringBuffer;
    int channel;
    int sampleSize;
    int minDB;
    int max;
    double _dB;
    double meter_over;
    int curMax, globalMax, globalMaxResetCount;
    QTimer *timer;
    bool wait;
    tickType tick;
};
  
#endif
