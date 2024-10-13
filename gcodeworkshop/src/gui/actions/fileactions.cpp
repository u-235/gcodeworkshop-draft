/*
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
 *  but WITHOUT ANY WARRANTY(new QAction(this)), without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QAction>          // for QAction
#include <QApplication>     // for qApp
#include <QIcon>            // for QIcon
#include <QLatin1String>    // for QLatin1String

#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/actions/knownactions.h>   // for KnownActions

#include "fileactions.h"    // for FileActions


using ActionId = GUI::Actions::KnownActions::File;


GUI::Actions::FileActions::FileActions(GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CLOSE), &QAction::triggered, app, &GCodeWorkShop::closeCurrentWindow);
	connect(makeAction(ActionId::CLOSE_ALL), &QAction::triggered, app, &GCodeWorkShop::closeAllMdiWindows);
	connect(makeAction(ActionId::EXIT), &QAction::triggered, qApp, &QApplication::closeAllWindows);
	connect(makeAction(ActionId::FIND_IN_FILES), &QAction::triggered, app, &GCodeWorkShop::findInFl);
	connect(makeAction(ActionId::NEW), &QAction::triggered, app, &GCodeWorkShop::newFileFromTemplate);
	connect(makeAction(ActionId::OPEN), &QAction::triggered, app, &GCodeWorkShop::open);
	connect(makeAction(ActionId::OPEN_EXAMPLE), &QAction::triggered, app, &GCodeWorkShop::openExample);
	connect(makeAction(ActionId::PRINT), &QAction::triggered, app, &GCodeWorkShop::printFile);
	connect(makeAction(ActionId::PRINT_PREVIEW), &QAction::triggered, app, &GCodeWorkShop::filePrintPreview);
	connect(makeAction(ActionId::SAVE), &QAction::triggered, app, &GCodeWorkShop::save);
	connect(makeAction(ActionId::SAVE_ALL), &QAction::triggered, app, &GCodeWorkShop::saveAll);
	connect(makeAction(ActionId::SAVE_AS), &QAction::triggered, app, &GCodeWorkShop::saveAs);
	connect(makeAction(ActionId::SESSION_MANAGER), &QAction::triggered, app, &GCodeWorkShop::showSessionDialog);

	connect(app, &GCodeWorkShop::updateIcons, this, &GUI::Actions::FileActions::loadIcons);
	connect(app, &GCodeWorkShop::updateTranslations, this, &GUI::Actions::FileActions::loadTranslations);
	connect(app, &GCodeWorkShop::updateShortcuts, this, &GUI::Actions::FileActions::loadShortcuts);

	GUI::Actions::FileActions::loadIcons();
	GUI::Actions::FileActions::loadTranslations();
}

GUI::Actions::FileActions::~FileActions()
{
}

void GUI::Actions::FileActions::loadTranslations()
{
	setActionText(ActionId::CLOSE, tr("Cl&ose \"%1\"").arg(""));
	setActionExToolTip(ActionId::CLOSE, tr("Close the active window"));
	setActionText(ActionId::CLOSE_ALL, tr("Close &All"));
	setActionExToolTip(ActionId::CLOSE_ALL, tr("Close all the windows"));
	setActionText(ActionId::EXIT, tr("E&xit"));
	setActionExToolTip(ActionId::EXIT, tr("Exit the application"));
	setActionText(ActionId::FIND_IN_FILES, tr("&Find files..."));
	setActionExToolTip(ActionId::FIND_IN_FILES, tr("Find files"));
	findFiles()->setCheckable(true);
	setActionText(ActionId::NEW, tr("&New"));
	setActionExToolTip(ActionId::NEW, tr("Create a new file"));
	setActionText(ActionId::OPEN, tr("&Open..."));
	setActionExToolTip(ActionId::OPEN, tr("Open an existing file"));
	setActionText(ActionId::OPEN_EXAMPLE, tr("&Open example..."));
	setActionExToolTip(ActionId::OPEN_EXAMPLE, tr("Open an example file"));
	setActionText(ActionId::PRINT, tr("&Print"));
	setActionExToolTip(ActionId::PRINT, tr("Print file"));
	setActionText(ActionId::PRINT_PREVIEW, tr("Pr&int preview"));
	setActionExToolTip(ActionId::PRINT_PREVIEW, tr("Preview printing"));
	setActionText(ActionId::SAVE, tr("&Save"));
	setActionExToolTip(ActionId::SAVE, tr("Save the document to disk"));
	setActionText(ActionId::SAVE_ALL, tr("Save A&ll"));
	setActionExToolTip(ActionId::SAVE_ALL, tr("Save all modified documents to disk"));
	setActionText(ActionId::SAVE_AS, tr("Save &As..."));
	setActionExToolTip(ActionId::SAVE_AS, tr("Save the document under a new name"));
	setActionText(ActionId::SESSION_MANAGER, tr("Session manager..."));
	setActionExToolTip(ActionId::SESSION_MANAGER, tr("Sessions manager"));
}

void GUI::Actions::FileActions::loadIcons()
{
	setActionIcon(ActionId::CLOSE, QIcon(":/images/fileclose.png"));
	setActionIcon(ActionId::CLOSE_ALL, QIcon(":/images/window-close.png"));
	setActionIcon(ActionId::EXIT, QIcon(":/images/exit.png"));
	setActionIcon(ActionId::FIND_IN_FILES, QIcon(":/images/filefind.png"));
	setActionIcon(ActionId::NEW, QIcon(":/images/filenew.png"));
	setActionIcon(ActionId::OPEN, QIcon(":/images/fileopen.png"));
	setActionIcon(ActionId::OPEN_EXAMPLE, QIcon(":/images/fileopen.png"));
	setActionIcon(ActionId::PRINT, QIcon(":/images/document-print.png"));
	setActionIcon(ActionId::PRINT_PREVIEW, QIcon(":/images/document-print-preview.png"));
	setActionIcon(ActionId::SAVE, QIcon(":/images/filesave.png"));
	setActionIcon(ActionId::SAVE_ALL, QIcon(":/images/filesaveall.png"));
	setActionIcon(ActionId::SAVE_AS, QIcon(":/images/filesaveas.png"));
}

QAction* GUI::Actions::FileActions::close()
{
	return action(ActionId::CLOSE);
}

QAction* GUI::Actions::FileActions::closeAll()
{
	return action(ActionId::CLOSE_ALL);
}

QAction* GUI::Actions::FileActions::exit()
{
	return action(ActionId::EXIT);
}

QAction* GUI::Actions::FileActions::findFiles()
{
	return action(ActionId::FIND_IN_FILES);
}

QAction* GUI::Actions::FileActions::makeNew()
{
	return action(ActionId::NEW);
}

QAction* GUI::Actions::FileActions::open()
{
	return action(ActionId::OPEN);
}

QAction* GUI::Actions::FileActions::openExample()
{
	return action(ActionId::OPEN_EXAMPLE);
}

QAction* GUI::Actions::FileActions::print()
{
	return action(ActionId::PRINT);
}

QAction* GUI::Actions::FileActions::printPreview()
{
	return action(ActionId::PRINT_PREVIEW);
}

QAction* GUI::Actions::FileActions::save()
{
	return action(ActionId::SAVE);
}

QAction* GUI::Actions::FileActions::saveAll()
{
	return action(ActionId::SAVE_ALL);
}

QAction* GUI::Actions::FileActions::saveAs()
{
	return action(ActionId::SAVE_AS);
}

QAction* GUI::Actions::FileActions::sessionManager()
{
	return action(ActionId::SESSION_MANAGER);
}
