/*
 *  Copyright (C) 2026 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of GCodeWorkShop.
 *
 *  GCodeWorkShop is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY(new QAction(this)), without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QAction>          // for QAction
#include <QIcon>            // for QIcon
#include <QKeySequence>     // for QTypeInfo<>::isComplex, QTypeInfo<>::isPointer
#include <QLatin1String>    // for QLatin1String
#include <QMainWindow>      // for QMainWindow
#include <QtGlobal>         // for QTypeInfo<>::isComplex, QTypeInfo<>::isRelocatable

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	#include <QMap>
#endif

#include <gcodefileserver.h>                // for GCodeFileServer
#include <gui/actions/actionkit.h>          // for ActionKit
#include <gui/fileserver/knownactions.h>    // for KnownActions

#include "gui/fileserver/windowactions.h"   // for WindowActions


using ActionId = GUI::FileServer::KnownActions::Window;


GUI::FileServer::WindowActions::WindowActions(GCodeFileServer* app, QMainWindow* mainWindow, QObject* parent) :
	ActionKit(parent)
{
	connect(makeAction(ActionId::ABOUT), &QAction::triggered, app, &GCodeFileServer::about);
	connect(makeAction(ActionId::BROWSE_SAVE_FOLDER), &QAction::triggered, app, &GCodeFileServer::browseSaveFolder);
	connect(makeAction(ActionId::CONFIGURE_PORT), &QAction::triggered, app, &GCodeFileServer::serialConfig);
	connect(makeAction(ActionId::MAXIMIZE), &QAction::triggered, mainWindow, &QMainWindow::showMaximized);
	connect(makeAction(ActionId::MINIMIZE), &QAction::triggered, mainWindow, &QMainWindow::hide);
	connect(makeAction(ActionId::QUIT), &QAction::triggered, app, &GCodeFileServer::quitApp);
	connect(makeAction(ActionId::RESTORE), &QAction::triggered, mainWindow, &QMainWindow::showNormal);
	connect(makeAction(ActionId::SERVER_CLOSE), &QAction::triggered, mainWindow, &QMainWindow::hide);
	connect(makeAction(ActionId::SERVER_RESET), &QAction::triggered, app, &GCodeFileServer::doPortReset);
	connect(makeAction(ActionId::SERVER_START), &QAction::triggered, app, &GCodeFileServer::startSerialPortServer);
	connect(makeAction(ActionId::SERVER_STOP), &QAction::triggered, app, &GCodeFileServer::stopSerialPortServer);
	connect(makeAction(ActionId::SHOW_NEW_FILES), &QAction::triggered, app, &GCodeFileServer::showNewFiles);
	makeAction(ActionId::START_MINIMIZED)->setCheckable(true);

	connect(app, &GCodeFileServer::updateIcons, this, &GUI::FileServer::WindowActions::loadIcons);
	connect(app, &GCodeFileServer::updateTranslations, this, &GUI::FileServer::WindowActions::loadTranslations);
	connect(app, &GCodeFileServer::updateShortcuts, this, & GUI::FileServer::WindowActions::loadShortcuts);

	GUI::FileServer::WindowActions::loadIcons();
	GUI::FileServer::WindowActions::loadTranslations();
}

GUI::FileServer::WindowActions::~WindowActions()
{
}

void GUI::FileServer::WindowActions::loadTranslations()
{
	setActionText(ActionId::ABOUT, tr("&About"));
	setActionExToolTip(ActionId::ABOUT, tr("Show the application's About box"));
	setActionText(ActionId::BROWSE_SAVE_FOLDER, tr("&Browse save folder"));
	setActionExToolTip(ActionId::BROWSE_SAVE_FOLDER, tr("Browse save folder"));
	setActionText(ActionId::CONFIGURE_PORT, tr("Serial port configuration"));
	setActionExToolTip(ActionId::CONFIGURE_PORT, tr("Serial port configuration"));
	setActionText(ActionId::MAXIMIZE, tr("Ma&ximize"));
	setActionExToolTip(ActionId::MAXIMIZE, "");
	setActionText(ActionId::MINIMIZE, tr("Mi&nimize"));
	setActionExToolTip(ActionId::MINIMIZE, "");
	setActionText(ActionId::QUIT, tr("&Quit"));
	setActionExToolTip(ActionId::QUIT, "");
	setActionText(ActionId::RESTORE, tr("&Restore"));
	setActionExToolTip(ActionId::RESTORE, "");
	setActionText(ActionId::SERVER_CLOSE, tr("&Minimize to tray"));
	setActionExToolTip(ActionId::SERVER_CLOSE, tr("Minimize to system tray"));
	setActionText(ActionId::SERVER_RESET, tr("&Reset"));
	setActionExToolTip(ActionId::SERVER_RESET, tr("Reset serial port"));
	setActionText(ActionId::SERVER_START, tr("&Start"));
	setActionExToolTip(ActionId::SERVER_START, tr("Start"));
	setActionText(ActionId::SERVER_STOP, tr("S&top"));
	setActionExToolTip(ActionId::SERVER_STOP, tr("Stop"));
	setActionText(ActionId::SHOW_NEW_FILES, tr("&Show saved files"));
	setActionExToolTip(ActionId::SHOW_NEW_FILES, tr("Show saved files"));
	setActionText(ActionId::START_MINIMIZED, tr("Start minimized"));
	setActionExToolTip(ActionId::START_MINIMIZED, "");
}

void GUI::FileServer::WindowActions::loadIcons()
{
	setActionIcon(ActionId::ABOUT, QIcon());
	setActionIcon(ActionId::BROWSE_SAVE_FOLDER, QIcon(":/images/browse.png"));
	setActionIcon(ActionId::CONFIGURE_PORT, QIcon(":/images/serialconfig.png"));
	setActionIcon(ActionId::MAXIMIZE, QIcon());
	setActionIcon(ActionId::MINIMIZE, QIcon());
	setActionIcon(ActionId::QUIT, QIcon(":/images/exit.png"));
	setActionIcon(ActionId::RESTORE, QIcon());
	setActionIcon(ActionId::SERVER_CLOSE, QIcon(":/images/window-close.png"));
	setActionIcon(ActionId::SERVER_RESET, QIcon(":/images/reset.png"));
	setActionIcon(ActionId::SERVER_START, QIcon(":/images/connect_creating.png"));
	setActionIcon(ActionId::SERVER_STOP, QIcon(":/images/stop.png"));
	setActionIcon(ActionId::SHOW_NEW_FILES, QIcon(":/images/project_new.png"));
	setActionIcon(ActionId::START_MINIMIZED, QIcon());
}

QAction* GUI::FileServer::WindowActions::about()
{
	return action(ActionId::ABOUT);
}

QAction* GUI::FileServer::WindowActions::browseSaveFolder()
{
	return action(ActionId::BROWSE_SAVE_FOLDER);
}

QAction* GUI::FileServer::WindowActions::configurePort()
{
	return action(ActionId::CONFIGURE_PORT);
}

QAction* GUI::FileServer::WindowActions::maximize()
{
	return action(ActionId::MAXIMIZE);
}

QAction* GUI::FileServer::WindowActions::minimize()
{
	return action(ActionId::MINIMIZE);
}

QAction* GUI::FileServer::WindowActions::quit()
{
	return action(ActionId::QUIT);
}

QAction* GUI::FileServer::WindowActions::restore()
{
	return action(ActionId::RESTORE);
}

QAction* GUI::FileServer::WindowActions::serverClose()
{
	return action(ActionId::SERVER_CLOSE);
}

QAction* GUI::FileServer::WindowActions::serverReset()
{
	return action(ActionId::SERVER_RESET);
}

QAction* GUI::FileServer::WindowActions::serverStart()
{
	return action(ActionId::SERVER_START);
}

QAction* GUI::FileServer::WindowActions::serverStop()
{
	return action(ActionId::SERVER_STOP);
}

QAction* GUI::FileServer::WindowActions::showNewFiles()
{
	return action(ActionId::SHOW_NEW_FILES);
}

QAction* GUI::FileServer::WindowActions::startMinimized()
{
	return action(ActionId::START_MINIMIZED);
}
