/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of GCodeWorkShop.                                   *
 *                                                                         *
 *   GCodeWorkShop is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QApplication>         // for main
#include <QMainWindow>          // for QMainWindow
#include <QObject>              // for QObject, SIGNAL, SLOT
#include <QString>              // for QString
#include <QtGlobal>             // for Q_INIT_RESOURCE

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	#include <QList>
	using QStringList = QList<QString>;
#endif

#include <gcodeworkshop.h>      // for GCodeWorkShop
#include <QtSingleApplication>  // for QtSingleApplication
#include <utils/medium.h>       // for Medium


int main(int argc, char* argv[])
{
	Q_INIT_RESOURCE(application);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	/* Attribute Qt::AA_EnableHighDpiScaling must be set before QCoreApplication is created. */
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif

	QtSingleApplication app("GCodeWorkShop", argc, argv);

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
	// https://community.kde.org/Guidelines_and_HOWTOs/Wayland_Porting_Notes#Application_Icon
	QGuiApplication::setDesktopFileName("gcodeworkshop");
#endif

	QString txMessage;
	QStringList args;

	for (int i = 1; i < argc; ++i) {
		QString a = QString::fromUtf8(argv[i]);
		txMessage += a;
		args.append(a);

		if (i < argc - 1) {
			txMessage += ";";
		}
	}

	if (app.sendMessage(txMessage)) {
		return 0;
	}

	Medium& medium = Medium::instance();
	medium.addTranslationUnit("kdiff3");
	medium.updateTranslation();

	GCodeWorkShop* gcws = GCodeWorkShop::instance();
	app.setActivationWindow(gcws->mainWindow(), false);
	QObject::connect(&app, &QtSingleApplication::messageReceived, gcws, &GCodeWorkShop::messReceived);
	QObject::connect(gcws, &GCodeWorkShop::needToShow, &app, &QtSingleApplication::activateWindow);
	gcws->launch(args);

	return app.exec();
}
