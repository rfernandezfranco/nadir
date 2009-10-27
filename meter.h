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

enum tickType { TICK_UP, TICK_DOWN };

class Meter : public QWidget
{
  Q_OBJECT

  private:
    RingBuffer *ringBuffer;
    int channel;
    int sampleSize;
    int minDB;
    int curMax, globalMax, globalMaxResetCount;
    QTimer *timer;
    tickType tick;

  protected:
    virtual void paintEvent(QPaintEvent *);

  public:
    Meter(tickType p_tick, RingBuffer *p_ringbuffer, int channelIndex,
      int p_sampleSize, int p_minDB, QWidget* parent=0);
    ~Meter();
    virtual QSize sizeHint() const;
    virtual QSizePolicy sizePolicy() const;

  public slots: 
    void updateMeter();
    void resetGlobalMax();
};
  
#endif
