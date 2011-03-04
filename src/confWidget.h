/*
 * Copyright (C) 2009 Juan Roldan Ruiz <juan.roldan@gmail.com>
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
 
#ifndef CONFWIDGET_H
#define CONFWIDGET_H

#include <QMainWindow>
#include <QtGui/QWidget>
#include <QWidget>
#include <math.h>
#include "microphone.h"
#include "ui_confWidget.h"

class ConfWidget : public QWidget
{
  Q_OBJECT

  public:
    ConfWidget( QWidget *parent = 0, Microphone *mic = 0 );
    void loadSettings();
    void closeEvent();
    QString backgroundColor();

  signals:
    void closing();

  private slots:
    void setColor();
    void save();
    void updateAudioSlider(double);
    void resetAudioBox();
    void setThreshold( int i );
    void setWaitTime( int i );
  
  private:
    Ui::ConfWidget ui;
    QString lineColor;
    Microphone *myMic;
    int threshold;
    unsigned int waitTime;
    bool waiting;
};

#endif // CONFWIDGET_H

