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
 
#include <QDebug>
#include <QSettings>
#include "mainWidget.h"
#include <QTranslator>
#include <QLocale>

//#define DEBUG

using namespace std;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Internationalitation
  QString locale = QLocale::system().name();
  QTranslator translator;
  translator.load(QString("i18n/") + locale);
  app.installTranslator(&translator);
  
  QApplication::setQuitOnLastWindowClosed(false);

  MainWidget win;
  QSettings settings;

  QCoreApplication::setOrganizationName( "siesta" );
  QCoreApplication::setOrganizationDomain( "nadir.sourceforge.net" );
  QCoreApplication::setApplicationName( "nadir" );
  
  settings.beginGroup( "mainWidget" );
  bool min = settings.value( "minimized", false ).toBool();
  bool hidden = settings.value( "hidden", false ).toBool();
  settings.endGroup();

  if( min )
    win.showMinimized();
  else
    if( hidden )
      win.hide();
    else
      win.show();

  return app.exec();
}
