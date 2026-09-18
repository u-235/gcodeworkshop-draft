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

#ifndef GCODEFILESERVER_H
#define GCODEFILESERVER_H

// Special for the moc_gcodefileserver.cpp & Qt6
#include <QKeySequence>     // IWYU pragma: keep
#include <QObject>          // for Q_OBJECT, slots
#include <QString>          // for QString

class QMainWindow;
template <class Key, class T> class QMap;
class QWidget;

namespace GUI {
class SerialTransmissionDialog;

namespace FileServer {
class MainWindow;
} // namespace FileServer
} // namespace GUI


class GCodeFileServer : public QObject
{
	Q_OBJECT

public:
	explicit GCodeFileServer(QWidget* parent = 0);
	~GCodeFileServer();

	QMainWindow* mainWindow();

	void launch();

	bool tryCloseApp();

signals:
	void updateTranslations();
	void updateIcons();
	void updateShortcuts(const QMap<QString, QKeySequence>&);

public slots:
	void messReceived(const QString& msg);
	void serialConfig();
	void startSerialPortServer();
	void stopSerialPortServer();
	void doPortReset();
	void showNewFiles();
	void browseSaveFolder();
	void quitApp();
	void about();

private slots:
	void tileSubWindowsVertycally();

private:
	bool closable;
	GUI::FileServer::MainWindow* m_mainWindow;

	GUI::SerialTransmissionDialog* createSerialPortServer(QString __config);

	void saveSettings();
	void loadSettings();
};

#endif // GCODEFILESERVER_H
