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
#include <ui/actions/actionkit.h>       // for ActionKit
#include <ui/actions/knownactions.h>    // for Ui::Actions::KnownActions

#include "findtoolbaractions.h"


Ui::FindToolBarActions::FindToolBarActions(::GCodeWorkShop* parent) : Actions::ActionKit(parent)
{
	makeAction(Ui::Actions::KnownActions::FindToolBar::CLOSE);
	makeAction(Ui::Actions::KnownActions::FindToolBar::FIND_NEXT);
	makeAction(Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS);
	makeAction(Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL);
	makeAction(Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT);
	makeAction(Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS);

	connect(parent, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(parent, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));

	FindToolBarActions::loadIcons();
	FindToolBarActions::loadTranslations();
}

Ui::FindToolBarActions::~FindToolBarActions()
{
}

void Ui::FindToolBarActions::loadTranslations()
{
	setActionText(Ui::Actions::KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionText(Ui::Actions::KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionText(Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionText(Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionText(Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionText(Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
	setActionExToolTip(Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
}

void Ui::FindToolBarActions::loadIcons()
{
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::FIND_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, QIcon(":/images/arrow-left.png"));
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL, QIcon(":/images/arrow-right-double.png"));
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, QIcon(":/images/arrow-left.png"));
}

QAction* Ui::FindToolBarActions::findClose()
{
	return action(Ui::Actions::KnownActions::FindToolBar::CLOSE);
}

QAction* Ui::FindToolBarActions::findNext()
{
	return action(Ui::Actions::KnownActions::FindToolBar::FIND_NEXT);
}

QAction* Ui::FindToolBarActions::findPrevious()
{
	return action(Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS);
}

QAction* Ui::FindToolBarActions::replaceAll()
{
	return action(Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL);
}

QAction* Ui::FindToolBarActions::replaceNext()
{
	return action(Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT);
}

QAction* Ui::FindToolBarActions::replacePrevious()
{
	return action(Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS);
}
