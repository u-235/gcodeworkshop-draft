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

#include <QAction>          // for QAction
#include <QCloseEvent>      // for QCloseEvent
#include <QComboBox>        // for QComboBox
#include <QDesktopServices> // for QDesktopServices
#include <QDialog>          // for QDialog
#include <QIcon>            // for QIcon
#include <QList>            // for QList
#include <QMdiArea>         // for QMdiArea
#include <QMdiSubWindow>    // for QMdiSubWindow
#include <QMenu>            // for QMenu
#include <QMessageBox>      // for QMessageBox
#include <QSettings>        // for QSettings
#include <QSignalMapper>    // for QSignalMapper
#include <QString>          // for QString, operator==
#include <QStringList>      // for QStringList
#include <QTabBar>          // for QTabBar
#include <QTimer>           // for QTimer
#include <QToolBar>         // for QToolBar
#include <QUrl>             // for QUrl
#include <QVariant>         // for QVariant
#include <QWidget>          // for QWidget
#include <Qt>               // for WidgetAttribute, ToolBarArea, WindowModality, WindowType
#include <QtGlobal>         // for QForeachContainer, qMakeForeachContainer, QTypeInfo<>::isLarge, QTypeI...

#include <gcodefileserver.h>                            // IWYU pragma: associated
#include <gui/serialport/serialportconfigdialog.h>      // for SerialPortConfigDialog
#include <gui/serialport/serialtransmissiondialog.h>    // for SerialTransmissionDialog
#include <utils/medium.h>                               // for Medium
#include <version.h>

#include "filechecker.h"                    // for FileChecker
#include "gui/fileserver/windowactions.h"   // for WindowActions
#include "ui_gcodefileserver.h"             // for Ui::GCodeFileServer


GCodeFileServer::GCodeFileServer(QWidget* parent) : QMainWindow(parent), ui(new Ui::GCodeFileServer)
{
	setAttribute(Qt::WA_DeleteOnClose);
	ui->setupUi(this);
	m_windowActions = new GUI::FileServer::WindowActions(this, this, this);

	setWindowModality(Qt::NonModal);

	setWindowTitle(tr("GCodeWorkShop - Serial port file server"));

	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
	        SLOT(activeWindowChanged(QMdiSubWindow*)));

	closable = false;
	ui->mdiArea->setViewMode(QMdiArea::TabbedView);
	QTabBar* tab = ui->mdiArea->findChild<QTabBar*>();

	if (tab) {
		//connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
		//tab->setTabsClosable(true);
		// The tabs might be very wide
		tab->setExpanding(false);
	}

	createActions();

	createTrayIcon();
}

GCodeFileServer::~GCodeFileServer()
{
	delete ui;
}

QMainWindow* GCodeFileServer::mainWindow()
{
	return this;
}

void GCodeFileServer::launch()
{
	loadSerialConfignames();
	configBox->adjustSize();

	loadSettings();

	if (m_windowActions->startMinimized()->isChecked()) {
		QTimer::singleShot(500, this, SLOT(hide()));
	} else {
		showNormal();
	}
}

bool GCodeFileServer::tryCloseApp()
{
	if (!closable) {
		if (trayIcon->isVisible()) {
			QMessageBox::information(mainWindow(), tr("GCodeWorkShop - Serial port file server"),
			                         tr("The program will keep running in the "
			                            "system tray. To terminate the program, "
			                            "choose <b>Quit</b> in the context menu "
			                            "of the system tray entry."));
			hide();
			return false;
		}
	}

	saveSettings();
	const QList<QMdiSubWindow*>& wlist = ui->mdiArea->subWindowList(QMdiArea::StackingOrder);

	for (QMdiSubWindow* window : wlist) {
		GUI::SerialTransmissionDialog* mdiChild = qobject_cast<GUI::SerialTransmissionDialog*>(window->widget());
		mdiChild->close();
		mdiChild->parentWidget()->close();
	}

	return true;
}

void GCodeFileServer::messReceived(const QString& msg)
{
	showNormal();
}

void GCodeFileServer::saveSettings()
{
	QStringList activeConfigs;

	foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
		GUI::SerialTransmissionDialog* mdiChild = qobject_cast<GUI::SerialTransmissionDialog*>(window->widget());

		if (mdiChild != nullptr) {
			activeConfigs.append(mdiChild->configName());
		}
	}

	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("CommApp");
	settings.setValue("StartMinimized", m_windowActions->startMinimized()->isChecked());
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

	m_windowActions->startMinimized()->setChecked(settings.value("StartMinimized", false).toBool());

	QStringList activeConfigs = settings.value("ActiveConfigs",
	                            (QStringList() << "")).toStringList();
	activeConfigs.removeDuplicates();
	activeConfigs.removeAll("");
	settings.endGroup();

	QStringList list = activeConfigs;
	QStringList::Iterator it = list.begin();

	while (it != list.end()) {
		GUI::SerialTransmissionDialog* spServer = createSerialPortServer(*it);
		spServer->show();
		++it;
	}
}

GUI::SerialTransmissionDialog* GCodeFileServer::createSerialPortServer(QString __config)
{
	GUI::SerialTransmissionDialog* spServer = new GUI::SerialTransmissionDialog(mainWindow(), Qt::SubWindow,
	        true);  // Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint
	spServer->setAttribute(Qt::WA_DeleteOnClose);
	ui->mdiArea->addSubWindow(spServer);

	spServer->startFileServer(__config);

	return spServer;
}

void GCodeFileServer::startSerialPortServer()
{
	QString _config = configBox->currentText();

	GUI::SerialTransmissionDialog* _existing = findMdiChild(_config);

	if (_existing) {
		_existing->setFocus();
	} else {
		GUI::SerialTransmissionDialog* spServer = createSerialPortServer(_config);
		//spServer->startFileServer(_config, true);
		spServer->show();
	}
}

void GCodeFileServer::stopSerialPortServer()
{
	QString _config = configBox->currentText();

	GUI::SerialTransmissionDialog* _existing = findMdiChild(_config);

	if (_existing != nullptr) {
		_existing->close();
		_existing->parentWidget()->close();
	}

	changeActiveWindow();
}

void GCodeFileServer::createActions()
{
	configBox = new QComboBox();
	configBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	configBox->setDuplicatesEnabled(false);

	connect(configBox, SIGNAL(currentIndexChanged(int)), SLOT(changeActiveWindow()));

	ui->toolBar->addWidget(configBox);
	ui->toolBar->addAction(m_windowActions->configurePort());
	ui->toolBar->addSeparator();
	ui->toolBar->addAction(m_windowActions->serverStart());
	ui->toolBar->addAction(m_windowActions->serverStop());

	ui->toolBar->addSeparator();
	ui->toolBar->addAction(m_windowActions->serverReset());

	fileToolBar = new QToolBar(tr("FileToolBar"));
	addToolBar(Qt::TopToolBarArea, fileToolBar);
	fileToolBar->setObjectName("FileToolBar");


	fileToolBar->addAction(m_windowActions->browseSaveFolder());
	fileToolBar->addAction(m_windowActions->showNewFiles());
	fileToolBar->addSeparator();
	fileToolBar->addAction(m_windowActions->serverClose());

	ui->menu_File->addAction(m_windowActions->startMinimized());
	ui->menu_File->addSeparator();
	//    ui->menu_File->addAction(m_windowActions->browseSaveFolder());
	//    ui->menu_File->addAction(m_windowActions->showNewFiles());
	//    ui->menu_File->addSeparator();
	ui->menu_File->addAction(m_windowActions->serverClose());
	ui->menu_File->addSeparator();
	ui->menu_File->addAction(m_windowActions->quit());

	ui->menu_Help->addAction(m_windowActions->about());

	//updateCurrentSerialConfig();
}

void GCodeFileServer::serialConfig()
{
	GUI::SerialPortConfigDialog* serialConfigDialog = new GUI::SerialPortConfigDialog(mainWindow(),
	        configBox->currentText());

	if (serialConfigDialog->exec() == QDialog::Accepted) {
		loadSerialConfignames();
	}
}

void GCodeFileServer::loadSerialConfignames()
{
	int id;
	QStringList list;
	QString item;

	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("SerialPortConfigs");

	configBox->clear();
	list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
	list.sort();
	configBox->addItems(list);
	item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
	id = configBox->findText(item);

	if (id >= 0) {
		configBox->setCurrentIndex(id);
	}

	settings.endGroup();
}

//void CommApp::updateCurrentSerialConfig()
//{
//    int id;
//    QDir dir;

//    bool hasMdiChild = (activeMdiChild() != nullptr);
//    if(hasMdiChild && (serialToolBar != nullptr))
//    {
//        dir.setPath(activeMdiChild()->filePath());
//        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//        dir.setSorting(QDir::Name);
//        dir.setNameFilters(QStringList("*.ini"));

//        QFileInfoList list = dir.entryInfoList();

//        if(!list.isEmpty())
//        {
//            QFileInfo name = list.at(0);
//            id = configBox->findText(name.baseName());
//            if(id >= 0)
//                configBox->setCurrentIndex(id);
//        }
//    }
//}

void GCodeFileServer::tileSubWindowsVertycally()
{
	//    if(ui->mdiArea->subWindowList().isEmpty())
	//        return;

	//    QPoint position(0, 0);
	//    foreach(QMdiSubWindow *window, ui->mdiArea->subWindowList())
	//    {
	//        QRect rect(0, 0, ui->mdiArea->width(),
	//                   ui->mdiArea->height() / ui->mdiArea->subWindowList().count());
	//        window->setGeometry(rect);
	//        window->move(position);
	//        position.setY(position.y() + window->height());
	//    }
}

GUI::SerialTransmissionDialog* GCodeFileServer::findMdiChild(const QString __config)
{
	foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
		GUI::SerialTransmissionDialog* mdiChild = qobject_cast<GUI::SerialTransmissionDialog*>(window->widget());

		if (mdiChild != nullptr)
			if (mdiChild->configName() == __config) {
				return mdiChild;
			}
	}

	return nullptr;
}

void GCodeFileServer::setActiveSubWindow(QWidget* window)
{
	if (!window) {
		return;
	}

	ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

GUI::SerialTransmissionDialog* GCodeFileServer::activeMdiChild()
{
	if (QMdiSubWindow* activeSubWindow = ui->mdiArea->activeSubWindow()) {
		return qobject_cast<GUI::SerialTransmissionDialog*>(activeSubWindow->widget());
	}

	return nullptr;
}

void GCodeFileServer::activeWindowChanged(QMdiSubWindow* window)
{
	if (!window) {
		return;
	}

	GUI::SerialTransmissionDialog* mdiChild = qobject_cast<GUI::SerialTransmissionDialog*>
	        (window->widget()); //activeMdiChild();

	if (mdiChild != nullptr) {
		mdiChild->setFocus();

		if (configBox) {
			int id = configBox->findText(mdiChild->configName());

			if (id >= 0) {
				configBox->setCurrentIndex(id);
			}
		}

		m_windowActions->configurePort()->setEnabled(false);
		m_windowActions->serverStart()->setEnabled(false);
		m_windowActions->serverReset()->setEnabled(true);
		m_windowActions->serverStop()->setEnabled(true);
	} else {
		m_windowActions->configurePort()->setEnabled(true);
		m_windowActions->serverStart()->setEnabled(true);
		m_windowActions->serverReset()->setEnabled(false);
		m_windowActions->serverStop()->setEnabled(false);
	}
}

void GCodeFileServer::changeActiveWindow()
{
	GUI::SerialTransmissionDialog* mdiChild = findMdiChild(configBox->currentText());

	if (mdiChild != nullptr) {
		mdiChild->setFocus();
		m_windowActions->configurePort()->setEnabled(false);
		m_windowActions->serverStart()->setEnabled(false);
		m_windowActions->serverReset()->setEnabled(true);
		m_windowActions->serverStop()->setEnabled(true);
	} else {
		m_windowActions->configurePort()->setEnabled(true);
		m_windowActions->serverStart()->setEnabled(true);
		m_windowActions->serverReset()->setEnabled(false);
		m_windowActions->serverStop()->setEnabled(false);
	}
}

void GCodeFileServer::closeTab(int i)
{
	QTabBar* tab = ui->mdiArea->findChild<QTabBar*>();

	if (tab != nullptr) {
		tab->removeTab(i);
	}
}

void GCodeFileServer::doPortReset()
{
	GUI::SerialTransmissionDialog* mdiChild = activeMdiChild();

	if (mdiChild != nullptr) {
		mdiChild->portReset();
	}
}

void GCodeFileServer::showNewFiles()
{
	GUI::SerialTransmissionDialog* mdiChild = activeMdiChild();

	if (mdiChild != nullptr) {
		QSettings& settings = *Medium::instance().settings();
		QStringList extensions = settings.value("Extensions", "").toStringList();
		extensions.removeDuplicates();
		extensions.sort();

		FileChecker* fileCheck = new FileChecker(this);
		fileCheck->setData(mdiChild->savePath(), mdiChild->readPaths(), extensions);
		fileCheck->findFiles();
		fileCheck->exec();
	}
}

void GCodeFileServer::browseSaveFolder()
{
	GUI::SerialTransmissionDialog* mdiChild = activeMdiChild();

	if (mdiChild != nullptr) {
		QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(mdiChild->savePath()),
		                               QUrl::TolerantMode));
	}
}

void GCodeFileServer::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(m_windowActions->minimize());
	trayIconMenu->addAction(m_windowActions->maximize());
	trayIconMenu->addAction(m_windowActions->restore());
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(m_windowActions->about());
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(m_windowActions->quit());

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
	        SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	trayIcon->setIcon(QIcon(":/images/serial.png"));
	trayIcon->show();
}

void GCodeFileServer::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		if (isVisible()) {
			hide();
		} else {
			showNormal();
		}

		break;

	case QSystemTrayIcon::DoubleClick:
		break;

	case QSystemTrayIcon::MiddleClick:
		break;

	default:
		;
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

void GCodeFileServer::closeEvent(QCloseEvent* event)
{
	event->setAccepted(tryCloseApp());
}
