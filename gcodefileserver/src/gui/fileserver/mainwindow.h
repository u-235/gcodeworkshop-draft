/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2024 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of GCodeWorkShop.
 *
 *  GCodeWorkShop is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QList>            // for QList
#include <QMainWindow>      // for QMainWindow
#include <QObject>          // for Q_OBJECT, slots
#include <QString>          // for QString
#include <QSystemTrayIcon>  // for QSystemTrayIcon

class QComboBox;
class QMdiSubWindow;
class QMenu;
class QToolBar;
class QWidget;

class GCodeFileServer;

namespace Ui {
class MainWindow;
}

namespace GUI {
class SerialTransmissionDialog;

namespace FileServer {
class WindowActions;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(GCodeFileServer* application, QWidget* parent = nullptr);
	~MainWindow();

	GCodeFileServer* app();
	bool isTrayVisible() const;
	bool isStartMinimized() const;
	void setStartMinimized(bool min);
	void loadSerialConfignames();
	QString currentConfigName() const;
	void addServerWidget(GUI::SerialTransmissionDialog* sw);
	GUI::SerialTransmissionDialog* activeServerWidget();
	GUI::SerialTransmissionDialog* serverWidgetByConfigName(const QString& config) const ;
	QList<GUI::SerialTransmissionDialog*> serverWidgetList() const;

public slots:
	void loadTranslations();
	void loadIcons();

	void changeActiveWindow();
	void activeWindowChanged(QMdiSubWindow* window);

protected slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::MainWindow* ui;
	GCodeFileServer* m_app;
	GUI::FileServer::WindowActions* m_windowActions;
	QComboBox* m_configBox;
	QToolBar* m_fileToolBar;
	QMenu* m_trayIconMenu;
	QSystemTrayIcon* m_trayIcon;
};
} // namespace FileServer
} // namespace GUI

#endif // GUI_MAINWINDOW_H
