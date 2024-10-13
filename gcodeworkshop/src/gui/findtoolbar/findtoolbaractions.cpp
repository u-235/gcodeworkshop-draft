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

#include <QIcon>            // for QIcon
#include <QLatin1String>    // for QLatin1String

#include <gcodeworkshop.h>
#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/actions/knownactions.h>   // for GUI::Actions::KnownActions

#include "findtoolbaractions.h"


GUI::FindToolBarActions::FindToolBarActions(GCodeWorkShop* parent) : Actions::ActionKit(parent)
{
	makeAction(GUI::Actions::KnownActions::FindToolBar::CLOSE);
	makeAction(GUI::Actions::KnownActions::FindToolBar::FIND_NEXT);
	makeAction(GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS);
	makeAction(GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL);
	makeAction(GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT);
	makeAction(GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS);

	connect(parent, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(parent, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));

	FindToolBarActions::loadIcons();
	FindToolBarActions::loadTranslations();
}

GUI::FindToolBarActions::~FindToolBarActions()
{
}

void GUI::FindToolBarActions::loadTranslations()
{
	setActionText(GUI::Actions::KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionText(GUI::Actions::KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionText(GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionText(GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionText(GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionText(GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
	setActionExToolTip(GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
}

void GUI::FindToolBarActions::loadIcons()
{
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::FIND_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, QIcon(":/images/arrow-left.png"));
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL, QIcon(":/images/arrow-right-double.png"));
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, QIcon(":/images/arrow-left.png"));
}

QAction* GUI::FindToolBarActions::findClose()
{
	return action(GUI::Actions::KnownActions::FindToolBar::CLOSE);
}

QAction* GUI::FindToolBarActions::findNext()
{
	return action(GUI::Actions::KnownActions::FindToolBar::FIND_NEXT);
}

QAction* GUI::FindToolBarActions::findPrevious()
{
	return action(GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS);
}

QAction* GUI::FindToolBarActions::replaceAll()
{
	return action(GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL);
}

QAction* GUI::FindToolBarActions::replaceNext()
{
	return action(GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT);
}

QAction* GUI::FindToolBarActions::replacePrevious()
{
	return action(GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS);
}
