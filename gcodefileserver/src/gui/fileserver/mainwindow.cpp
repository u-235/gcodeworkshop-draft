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

#include <QAction>          // for QAction
#include <QCloseEvent>      // for QCloseEvent
#include <QComboBox>        // for QComboBox
#include <QIcon>            // for QIcon
#include <QList>            // for QList
#include <QMdiArea>         // for QMdiArea
#include <QMdiSubWindow>    // for QMdiSubWindow
#include <QMenu>            // for QMenu
#include <QSettings>        // for QSettings
#include <QString>          // for QString, operator==
#include <QSystemTrayIcon>  // for QSystemTrayIcon
#include <QTabBar>          // for QTabBar
#include <QToolBar>         // for QToolBar
#include <QVariant>         // for QVariant
#include <QWidget>          // for QWidget
#include <Qt>               // for ToolBarArea, WidgetAttribute, WindowModality
#include <QtGlobal>         // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QTypeInfo<...

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	using QStringList = QList<QString>;
#endif

#include <gui/serialport/serialtransmissiondialog.h>    // for SerialTransmissionDialog
#include <utils/medium.h>                               // for Medium

#include "gcodefileserver.h"
#include "gui/fileserver/mainwindow.h"      // IWYU pragma: associated
#include "gui/fileserver/windowactions.h"   //
#include "ui_mainwindow.h"                  // for Ui::MainWindow


GUI::FileServer::MainWindow::MainWindow(GCodeFileServer* application, QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_app(application),
	m_windowActions(new GUI::FileServer::WindowActions(application, this, this)),
	m_configBox(new QComboBox(this)),
	m_fileToolBar(new QToolBar(this)),
	m_trayIconMenu(new QMenu(this)),
	m_trayIcon(new QSystemTrayIcon(this))
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::NonModal);

	ui->mdiArea->setViewMode(QMdiArea::TabbedView);
	QTabBar* tab = ui->mdiArea->findChild<QTabBar*>();
	connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::activeWindowChanged);

	if (tab) {
		tab->setExpanding(false);
	}

	m_configBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_configBox->setDuplicatesEnabled(false);
	connect(m_configBox, &QComboBox::currentTextChanged, this, &MainWindow::changeActiveWindow);

	ui->toolBar->addWidget(m_configBox);
	ui->toolBar->addAction(m_windowActions->configurePort());
	ui->toolBar->addSeparator();
	ui->toolBar->addAction(m_windowActions->serverStart());
	ui->toolBar->addAction(m_windowActions->serverStop());
	ui->toolBar->addSeparator();
	ui->toolBar->addAction(m_windowActions->serverReset());

	addToolBar(Qt::TopToolBarArea, m_fileToolBar);
	m_fileToolBar->setObjectName("FileToolBar");
	m_fileToolBar->addAction(m_windowActions->browseSaveFolder());
	m_fileToolBar->addAction(m_windowActions->showNewFiles());
	m_fileToolBar->addSeparator();
	m_fileToolBar->addAction(m_windowActions->serverClose());

	ui->menuFile->addAction(m_windowActions->startMinimized());
	ui->menuFile->addSeparator();
	ui->menuFile->addAction(m_windowActions->serverClose());
	ui->menuFile->addSeparator();
	ui->menuFile->addAction(m_windowActions->quit());

	ui->menuHelp->addAction(m_windowActions->about());

	m_trayIconMenu->addAction(m_windowActions->minimize());
	m_trayIconMenu->addAction(m_windowActions->maximize());
	m_trayIconMenu->addAction(m_windowActions->restore());
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(m_windowActions->about());
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(m_windowActions->quit());

	m_trayIcon->setContextMenu(m_trayIconMenu);
	connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
	m_trayIcon->show();

	loadIcons();
	loadTranslations();
}

GUI::FileServer::MainWindow::~MainWindow()
{
	delete ui;
}

bool GUI::FileServer::MainWindow::isTrayVisible() const
{
	return m_trayIcon->isVisible();
}

bool GUI::FileServer::MainWindow::isStartMinimized() const
{
	return m_windowActions->startMinimized()->isChecked();
}

void GUI::FileServer::MainWindow::setStartMinimized(bool min)
{
	m_windowActions->startMinimized()->setChecked(min);
}

void GUI::FileServer::MainWindow::loadSerialConfignames()
{
	int id;
	QStringList list;
	QString item;

	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("SerialPortConfigs");

	m_configBox->clear();
	list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
	list.sort();
	m_configBox->addItems(list);
	item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
	id = m_configBox->findText(item);

	if (id >= 0) {
		m_configBox->setCurrentIndex(id);
	}

	settings.endGroup();
	m_configBox->adjustSize();
}

QString GUI::FileServer::MainWindow::currentConfigName() const
{
	return m_configBox->currentText();
}

void GUI::FileServer::MainWindow::addServerWidget(SerialTransmissionDialog* sw)
{
	ui->mdiArea->addSubWindow(sw);
}

GUI::SerialTransmissionDialog* GUI::FileServer::MainWindow::activeServerWidget()
{
	if (QMdiSubWindow* activeSubWindow = ui->mdiArea->activeSubWindow()) {
		return qobject_cast<GUI::SerialTransmissionDialog*>(activeSubWindow->widget());
	}

	return nullptr;
}

GUI::SerialTransmissionDialog* GUI::FileServer::MainWindow::serverWidgetByConfigName(const QString& config) const
{
	const QList<GUI::SerialTransmissionDialog*>& swList = serverWidgetList();

	for (GUI::SerialTransmissionDialog* sw : swList) {
		if (sw->configName() == config) {
			return sw;
		}
	}

	return nullptr;
}

QList<GUI::SerialTransmissionDialog*> GUI::FileServer::MainWindow::serverWidgetList() const
{
	const QList<QMdiSubWindow*>& wlist = ui->mdiArea->subWindowList(QMdiArea::StackingOrder);
	QList<GUI::SerialTransmissionDialog*> serverWidgets;

	for (QMdiSubWindow* window : wlist) {
		GUI::SerialTransmissionDialog* sw = qobject_cast<GUI::SerialTransmissionDialog*>(window->widget());

		if (sw != nullptr) {
			serverWidgets.append(sw);
		}
	}

	return serverWidgets;
}

void GUI::FileServer::MainWindow::loadTranslations()
{
	setWindowTitle(tr("GCodeWorkShop - Serial port file server"));
	m_fileToolBar->setWindowTitle(tr("FileToolBar"));
	ui->retranslateUi(this);
}

void GUI::FileServer::MainWindow::loadIcons()
{
	setWindowIcon(QIcon(":/images/serial.png"));
	m_trayIcon->setIcon(QIcon(":/images/serial.png"));
}

void GUI::FileServer::MainWindow::changeActiveWindow()
{
	GUI::SerialTransmissionDialog* sw = serverWidgetByConfigName(currentConfigName());

	if (sw != nullptr) {
		sw->setFocus();
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

void GUI::FileServer::MainWindow::activeWindowChanged(QMdiSubWindow* window)
{
	if (!window) {
		return;
	}

	GUI::SerialTransmissionDialog* sw = qobject_cast<GUI::SerialTransmissionDialog*> (window->widget());

	if (sw != nullptr) {
		sw->setFocus();

		if (m_configBox) {
			int id = m_configBox->findText(sw->configName());

			if (id >= 0) {
				m_configBox->setCurrentIndex(id);
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

void GUI::FileServer::MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		if (isVisible()) {
			hide();
		} else {
			showNormal();
		}

		break;

	default:
		;
	}
}

void GUI::FileServer::MainWindow::closeEvent(QCloseEvent* event)
{
	event->setAccepted(m_app->tryCloseApp());
}
