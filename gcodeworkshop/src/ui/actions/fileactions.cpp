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
#include <ui/actions/actionkit.h>       // for ActionKit
#include <ui/actions/knownactions.h>    // for KnownActions

#include "fileactions.h"    // for FileActions


using ActionId = Ui::Actions::KnownActions::File;


Ui::Actions::FileActions::FileActions(::GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CLOSE), SIGNAL(triggered()), app, SLOT(closeCurrentWindow()));
	connect(makeAction(ActionId::CLOSE_ALL), SIGNAL(triggered()), app, SLOT(closeAllMdiWindows()));
	connect(makeAction(ActionId::EXIT), SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	connect(makeAction(ActionId::FIND_IN_FILES), SIGNAL(triggered()), app, SLOT(findInFl()));
	connect(makeAction(ActionId::NEW), SIGNAL(triggered()), app, SLOT(newFileFromTemplate()));
	connect(makeAction(ActionId::OPEN), SIGNAL(triggered()), app, SLOT(open()));
	connect(makeAction(ActionId::OPEN_EXAMPLE), SIGNAL(triggered()), app, SLOT(openExample()));
	connect(makeAction(ActionId::PRINT), SIGNAL(triggered()), app, SLOT(printFile()));
	connect(makeAction(ActionId::PRINT_PREVIEW), SIGNAL(triggered()), app, SLOT(filePrintPreview()));
	connect(makeAction(ActionId::SAVE), SIGNAL(triggered()), app, SLOT(save()));
	connect(makeAction(ActionId::SAVE_ALL), SIGNAL(triggered()), app, SLOT(saveAll()));
	connect(makeAction(ActionId::SAVE_AS), SIGNAL(triggered()), app, SLOT(saveAs()));
	connect(makeAction(ActionId::SESSION_MANAGER), SIGNAL(triggered()), app, SLOT(showSessionDialog()));

	connect(app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(app, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	Ui::Actions::FileActions::loadIcons();
	Ui::Actions::FileActions::loadTranslations();
}

Ui::Actions::FileActions::~FileActions()
{
}

void Ui::Actions::FileActions::loadTranslations()
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

void Ui::Actions::FileActions::loadIcons()
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

QAction* Ui::Actions::FileActions::close()
{
	return action(ActionId::CLOSE);
}

QAction* Ui::Actions::FileActions::closeAll()
{
	return action(ActionId::CLOSE_ALL);
}

QAction* Ui::Actions::FileActions::exit()
{
	return action(ActionId::EXIT);
}

QAction* Ui::Actions::FileActions::findFiles()
{
	return action(ActionId::FIND_IN_FILES);
}

QAction* Ui::Actions::FileActions::makeNew()
{
	return action(ActionId::NEW);
}

QAction* Ui::Actions::FileActions::open()
{
	return action(ActionId::OPEN);
}

QAction* Ui::Actions::FileActions::openExample()
{
	return action(ActionId::OPEN_EXAMPLE);
}

QAction* Ui::Actions::FileActions::print()
{
	return action(ActionId::PRINT);
}

QAction* Ui::Actions::FileActions::printPreview()
{
	return action(ActionId::PRINT_PREVIEW);
}

QAction* Ui::Actions::FileActions::save()
{
	return action(ActionId::SAVE);
}

QAction* Ui::Actions::FileActions::saveAll()
{
	return action(ActionId::SAVE_ALL);
}

QAction* Ui::Actions::FileActions::saveAs()
{
	return action(ActionId::SAVE_AS);
}

QAction* Ui::Actions::FileActions::sessionManager()
{
	return action(ActionId::SESSION_MANAGER);
}
