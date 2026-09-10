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

#include <QDesktopServices> // for QDesktopServices
#include <QDialog>          // for QDialog
#include <QList>            // for QList
#include <QMainWindow>      // for QMainWindow
#include <QMessageBox>      // for QMessageBox
#include <QSettings>        // for QSettings
#include <QString>          // for QString
#include <QStringList>      // for QStringList
#include <QTimer>           // for QTimer
#include <QUrl>             // for QUrl
#include <QVariant>         // for QVariant
#include <QWidget>          // for QWidget
#include <Qt>               // for WidgetAttribute, WindowType
#include <QtGlobal>         // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic

#include <gcodefileserver.h>                            // IWYU pragma: associated
#include <gui/serialport/serialportconfigdialog.h>      // for SerialPortConfigDialog
#include <gui/serialport/serialtransmissiondialog.h>    // for SerialTransmissionDialog
#include <utils/medium.h>                               // for Medium
#include <version.h>

#include "gui/filechecker/filechecker.h"    // for FileChecker
#include "gui/fileserver/mainwindow.h"      // for MainWindow


GCodeFileServer::GCodeFileServer(QWidget* parent) :
	QObject(parent),
	closable(false),
	m_mainWindow(new GUI::FileServer::MainWindow(this, nullptr))
{
	connect(this, &GCodeFileServer::updateIcons, m_mainWindow, &GUI::FileServer::MainWindow::loadIcons);
	connect(this, &GCodeFileServer::updateTranslations, m_mainWindow, &GUI::FileServer::MainWindow::loadTranslations);
}

GCodeFileServer::~GCodeFileServer()
{
	m_mainWindow->deleteLater();
}

QMainWindow* GCodeFileServer::mainWindow()
{
	return m_mainWindow;
}

void GCodeFileServer::launch()
{
	m_mainWindow->loadSerialConfignames();
	loadSettings();

	if (m_mainWindow->isStartMinimized()) {
		QTimer::singleShot(500, mainWindow(), &QMainWindow::hide);
	} else {
		mainWindow()->showNormal();
	}
}

bool GCodeFileServer::tryCloseApp()
{
	if (!closable) {
		if (m_mainWindow->isTrayVisible()) {
			QMessageBox::information(mainWindow(), tr("GCodeWorkShop - Serial port file server"),
			                         tr("The program will keep running in the "
			                            "system tray. To terminate the program, "
			                            "choose <b>Quit</b> in the context menu "
			                            "of the system tray entry."));
			mainWindow()->hide();
			return false;
		}
	}

	saveSettings();
	const QList<GUI::SerialTransmissionDialog*>& wlist = m_mainWindow->serverWidgetList();

	for (GUI::SerialTransmissionDialog* sw : wlist) {
		sw->close();
		sw->parentWidget()->close();
	}

	return true;
}

void GCodeFileServer::messReceived(const QString& msg)
{
	mainWindow()->showNormal();
}

void GCodeFileServer::saveSettings()
{
	QStringList activeConfigs;

	const QList<GUI::SerialTransmissionDialog*>& wlist = m_mainWindow->serverWidgetList();

	for (GUI::SerialTransmissionDialog* sw : wlist) {
		activeConfigs.append(sw->configName());
	}

	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("CommApp");
	settings.setValue("StartMinimized", m_mainWindow->isStartMinimized());
	settings.remove("ActiveConfigs");
	activeConfigs.removeDuplicates();
	activeConfigs.removeAll("");
	settings.setValue("ActiveConfigs", activeConfigs);
	settings.endGroup();
}

void GCodeFileServer::loadSettings()
{
	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("CommApp");

	m_mainWindow->setStartMinimized(settings.value("StartMinimized", false).toBool());

	QStringList activeConfigs = settings.value("ActiveConfigs",
	                            (QStringList() << "")).toStringList();
	activeConfigs.removeDuplicates();
	activeConfigs.removeAll("");
	settings.endGroup();

	QStringList list = activeConfigs;
	QStringList::Iterator it = list.begin();

	while (it != list.end()) {
		auto sw = createSerialPortServer(*it);
		sw->show();
		++it;
	}
}

GUI::SerialTransmissionDialog* GCodeFileServer::createSerialPortServer(const QString& config)
{
	auto sw = new GUI::SerialTransmissionDialog(mainWindow(), Qt::SubWindow, true);
	sw->setAttribute(Qt::WA_DeleteOnClose);
	m_mainWindow->addServerWidget(sw);
	sw->startFileServer(config);
	return sw;
}

void GCodeFileServer::startSerialPortServer()
{
	const QString& config = m_mainWindow->currentConfigName();
	auto sw = m_mainWindow->serverWidgetByConfigName(config);

	if (sw != nullptr) {
		sw->setFocus();
	} else {
		sw = createSerialPortServer(config);
		sw->show();
	}
}

void GCodeFileServer::stopSerialPortServer()
{
	const QString& config = m_mainWindow->currentConfigName();
	auto sw = m_mainWindow->serverWidgetByConfigName(config);

	if (sw != nullptr) {
		sw->close();
		sw->parentWidget()->close();
	}

	m_mainWindow->changeActiveWindow();
}

void GCodeFileServer::serialConfig()
{
	const QString& config = m_mainWindow->currentConfigName();
	auto serialConfigDialog = new GUI::SerialPortConfigDialog(mainWindow(), config);

	if (serialConfigDialog->exec() == QDialog::Accepted) {
		m_mainWindow->loadSerialConfignames();
	}
}

void GCodeFileServer::doPortReset()
{
	auto sw = m_mainWindow->activeServerWidget();

	if (sw != nullptr) {
		sw->portReset();
	}
}

void GCodeFileServer::showNewFiles()
{
	auto sw = m_mainWindow->activeServerWidget();

	if (sw != nullptr) {
		QSettings& settings = *Medium::instance().settings();
		QStringList extensions = settings.value("Extensions", "").toStringList();
		extensions.removeDuplicates();
		extensions.sort();

		GUI::FileChecker* fileCheck = new GUI::FileChecker(mainWindow());
		fileCheck->setData(sw->savePath(), sw->readPaths(), extensions);
		fileCheck->findFiles();
		fileCheck->exec();
	}
}

void GCodeFileServer::browseSaveFolder()
{
	auto sw = m_mainWindow->activeServerWidget();

	if (sw != nullptr) {
		QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(sw->savePath()), QUrl::TolerantMode));
	}
}

void GCodeFileServer::quitApp()
{
	closable = true;
	mainWindow()->close();
}

void GCodeFileServer::about()
{
	QMessageBox::about(mainWindow(), tr("About GCodeFileServer"),
	                   tr("The <b>GCodeFileServer</b> is part of GCodeWorkShop."
	                      "<P>Version: %1"
	                      "<P>Copyright (C) 1998 - 2015 by <a href=\"mailto:artkoz78@gmail.com\">Artur Kozioł</a>"
	                      "<br>Copyright (C) 2020 - 2025 by <a href='mailto:nicegorov@yandex.ru'>Nick Egorrov</a>"
	                      "<P>GCodeFileServer contains pieces of code from other Open Source projects."
	                      "<P><i>GCodeFileServer is free software; you can redistribute it and/or modify"
	                      "it under the terms of the GNU General Public License  as published by"
	                      "the Free Software Foundation; either version 2 of the License, or"
	                      "(at your option) any later version.</i>"
	                      "<P><i>The program is provided AS IS with NO WARRANTY OF ANY KIND,"
	                      "INCLUDING THE WARRANTY OF DESIGN,"
	                      "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</i>").arg(GCODEWORKSHOP_VERSION));
}
