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
#include <gui/workshop/knownactions.h>  // for GUI::Actions::KnownActions

#include "findtoolbaractions.h"


using ActionId = GUI::WorkShop::KnownActions::FindToolBar;


GUI::FindToolBarActions::FindToolBarActions(GCodeWorkShop* parent) : Actions::ActionKit(parent)
{
	makeAction(ActionId::CLOSE);
	makeAction(ActionId::FIND_NEXT);
	makeAction(ActionId::FIND_PREVIOUS);
	makeAction(ActionId::REPLACE_ALL);
	makeAction(ActionId::REPLACE_NEXT);
	makeAction(ActionId::REPLACE_PREVIOUS);

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
	setActionText(ActionId::CLOSE, tr("Close find toolbar"));
	setActionExToolTip(ActionId::CLOSE, tr("Close find toolbar"));
	setActionText(ActionId::FIND_NEXT, tr("Find next"));
	setActionExToolTip(ActionId::FIND_NEXT, tr("Find next"));
	setActionText(ActionId::FIND_PREVIOUS, tr("Find previous"));
	setActionExToolTip(ActionId::FIND_PREVIOUS, tr("Find previous"));
	setActionText(ActionId::REPLACE_ALL, tr("Replace all"));
	setActionExToolTip(ActionId::REPLACE_ALL, tr("Replace all"));
	setActionText(ActionId::REPLACE_NEXT, tr("Replace && find next"));
	setActionExToolTip(ActionId::REPLACE_NEXT, tr("Replace && find next"));
	setActionText(ActionId::REPLACE_PREVIOUS, tr("Replace && find previous"));
	setActionExToolTip(ActionId::REPLACE_PREVIOUS, tr("Replace && find previous"));
}

void GUI::FindToolBarActions::loadIcons()
{
	setActionIcon(ActionId::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(ActionId::FIND_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(ActionId::FIND_PREVIOUS, QIcon(":/images/arrow-left.png"));
	setActionIcon(ActionId::REPLACE_ALL, QIcon(":/images/arrow-right-double.png"));
	setActionIcon(ActionId::REPLACE_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(ActionId::REPLACE_PREVIOUS, QIcon(":/images/arrow-left.png"));
}

QAction* GUI::FindToolBarActions::findClose()
{
	return action(ActionId::CLOSE);
}

QAction* GUI::FindToolBarActions::findNext()
{
	return action(ActionId::FIND_NEXT);
}

QAction* GUI::FindToolBarActions::findPrevious()
{
	return action(ActionId::FIND_PREVIOUS);
}

QAction* GUI::FindToolBarActions::replaceAll()
{
	return action(ActionId::REPLACE_ALL);
}

QAction* GUI::FindToolBarActions::replaceNext()
{
	return action(ActionId::REPLACE_NEXT);
}

QAction* GUI::FindToolBarActions::replacePrevious()
{
	return action(ActionId::REPLACE_PREVIOUS);
}
