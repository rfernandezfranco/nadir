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
#include "ui_confWidget.h"

class ConfWidget : public QWidget
{
  Q_OBJECT

  public:
    ConfWidget( QWidget *parent = 0 );
    void loadSettings();
		void closeEvent();
		QString backgroundColor();
/*
    void getScreenSize();
    void setScreenWidth( int w );
    int getScreenWidth( void );
    void setScreenHeight( int h );
    int getScreenHeight( void );
    void setSpeed( int s );
    int getSpeed( void );

    bool active;
    bool continuous;
    ScanLine *hLine;
    ScanLine *vLine;

  protected:
    void keyPressEvent(QKeyEvent *event );
*/
	signals:
		void closing();
  private slots:
    void setColor();
    void save();
  
  private:
    Ui::ConfWidget ui;
		QString lineColor;
    /*
    enum { LEFT=0, RIGHT, DOUBLE, DRAG, DROP } mouseEvent;
    enum { STOP=0, START, SCAN1, SCAN2, EVENT } state;
    int x;
    int y;
    int screenWidth;
    int screenHeight;
    int speed;
    int lineWidth;
    */
};

#endif // CONFWIDGET_H
