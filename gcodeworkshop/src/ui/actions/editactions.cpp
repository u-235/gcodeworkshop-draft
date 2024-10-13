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
#include <QIcon>            // for QIcon
#include <QLatin1String>    // for QLatin1String

#include <gcodeworkshop.h>
#include <ui/actions/actionkit.h>       // for ActionKit
#include <ui/actions/knownactions.h>    // for KnownActions

#include "editactions.h"    // for EditAcions


using ActionId = Ui::Actions::KnownActions::Edit;


Ui::Actions::EditActions::EditActions(::GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CONFIG), SIGNAL(triggered()), app, SLOT(config()));
	connect(makeAction(ActionId::COPY), SIGNAL(triggered()), app, SLOT(copy()));
	connect(makeAction(ActionId::CUT), SIGNAL(triggered()), app, SLOT(cut()));
	connect(makeAction(ActionId::DELETE), SIGNAL(triggered()), app, SLOT(deleteText()));
	connect(makeAction(ActionId::FIND), SIGNAL(triggered()), app, SLOT(showFindToolBar()));
	connect(makeAction(ActionId::PASTE), SIGNAL(triggered()), app, SLOT(paste()));
	connect(makeAction(ActionId::READ_ONLY), SIGNAL(triggered()), app, SLOT(readOnly()));
	connect(makeAction(ActionId::REDO), SIGNAL(triggered()), app, SLOT(redo()));
	connect(makeAction(ActionId::REPLACE), SIGNAL(triggered()), app, SLOT(showReplaceToolBar()));
	connect(makeAction(ActionId::SELECT_ALL), SIGNAL(triggered()), app, SLOT(selAll()));
	connect(makeAction(ActionId::UNDO), SIGNAL(triggered()), app, SLOT(undo()));

	connect(app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(app, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	readOnly()->setCheckable(true);
	redo()->setEnabled(false);
	undo()->setEnabled(false);

	Ui::Actions::EditActions::loadIcons();
	Ui::Actions::EditActions::loadTranslations();
}

Ui::Actions::EditActions::~EditActions()
{
}

void Ui::Actions::EditActions::loadTranslations()
{
	setActionText(ActionId::CONFIG, tr("Configuration"));
	setActionExToolTip(ActionId::CONFIG, tr("Open configuration dialog"));
	setActionText(ActionId::COPY, tr("&Copy"));
	setActionExToolTip(ActionId::COPY, tr("Copy the current selection's contents to the clipboard"));
	setActionText(ActionId::CUT, tr("Cu&t"));
	setActionExToolTip(ActionId::CUT, tr("Cut the current selection's contents to the clipboard"));
	setActionText(ActionId::DELETE, tr("&Delete"));
	setActionExToolTip(ActionId::DELETE, tr("Removes selected text"));
	setActionText(ActionId::FIND, tr("&Find"));
	setActionExToolTip(ActionId::FIND, tr("Find text"));
	setActionText(ActionId::PASTE, tr("&Paste"));
	setActionExToolTip(ActionId::PASTE, tr("Paste the clipboard's contents into the current selection"));
	setActionText(ActionId::READ_ONLY, tr("Read &only"));
	setActionExToolTip(ActionId::READ_ONLY, tr("Makes text read only"));
	setActionText(ActionId::REDO, tr("&Redo"));
	setActionExToolTip(ActionId::REDO, tr("Redo last operation"));
	setActionText(ActionId::REPLACE, tr("&Replace"));
	setActionExToolTip(ActionId::REPLACE, tr("Find and replace text"));
	setActionText(ActionId::SELECT_ALL, tr("&Select all"));
	setActionExToolTip(ActionId::SELECT_ALL, tr("Select all text"));
	setActionText(ActionId::UNDO, tr("&Undo"));
	setActionExToolTip(ActionId::UNDO, tr("Undo last operation"));
}

void Ui::Actions::EditActions::loadIcons()
{
	setActionIcon(ActionId::CONFIG, QIcon(":/images/configure.png"));
	setActionIcon(ActionId::COPY, QIcon(":/images/editcopy.png"));
	setActionIcon(ActionId::CUT, QIcon(":/images/editcut.png"));
	setActionIcon(ActionId::DELETE, QIcon(":/images/editdelete.png"));
	setActionIcon(ActionId::FIND, QIcon(":/images/find.png"));
	setActionIcon(ActionId::PASTE, QIcon(":/images/editpaste.png"));
	setActionIcon(ActionId::READ_ONLY, QIcon(":/images/unlock.png"));
	setActionIcon(ActionId::REDO, QIcon(":/images/redo.png"));
	setActionIcon(ActionId::REPLACE, QIcon(":/images/replace.png"));
	setActionIcon(ActionId::SELECT_ALL, QIcon(":/images/edit-select-all.png"));
	setActionIcon(ActionId::UNDO, QIcon(":/images/undo.png"));
}

QAction* Ui::Actions::EditActions::config()
{
	return action(ActionId::CONFIG);
}

QAction* Ui::Actions::EditActions::copy()
{
	return action(ActionId::COPY);
}

QAction* Ui::Actions::EditActions::cut()
{
	return action(ActionId::CUT);
}

QAction* Ui::Actions::EditActions::del()
{
	return action(ActionId::DELETE);
}

QAction* Ui::Actions::EditActions::find()
{
	return action(ActionId::FIND);
}

QAction* Ui::Actions::EditActions::paste()
{
	return action(ActionId::PASTE);
}

QAction* Ui::Actions::EditActions::readOnly()
{
	return action(ActionId::READ_ONLY);
}

QAction* Ui::Actions::EditActions::redo()
{
	return action(ActionId::REDO);
}

QAction* Ui::Actions::EditActions::replace()
{
	return action(ActionId::REPLACE);
}

QAction* Ui::Actions::EditActions::selectAll()
{
	return action(ActionId::SELECT_ALL);
}

QAction* Ui::Actions::EditActions::undo()
{
	return action(ActionId::UNDO);
}
