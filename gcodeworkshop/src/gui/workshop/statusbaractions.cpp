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
#include <QLatin1String>    // for QLatin1String

#include <gcodeworkshop.h>
#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/workshop/knownactions.h>  // for KnownActions

#include "statusbaractions.h"


using ActionId = GUI::WorkShop::KnownActions::StatusBar;


GUI::WorkShop::StatusBarActions::StatusBarActions(GCodeWorkShop* app) : Actions::ActionKit(app)
{
	makeAction(ActionId::ATTACH_HIGHLIGHT_TO_DIR);
	makeAction(ActionId::DE_ATTACH_HIGHLIGHT_TO_DIR);

	connect(app, &GCodeWorkShop::updateIcons, this, &StatusBarActions::loadIcons);
	connect(app, &GCodeWorkShop::updateTranslations, this, &StatusBarActions::loadTranslations);

	StatusBarActions::loadIcons();
	StatusBarActions::loadTranslations();
}

GUI::WorkShop::StatusBarActions::~StatusBarActions()
{
}

void GUI::WorkShop::StatusBarActions::loadTranslations()
{
	setActionText(ActionId::ATTACH_HIGHLIGHT_TO_DIR,
	              tr("Attach current highlight setting to current directory of programs"));
	setActionExToolTip(ActionId::ATTACH_HIGHLIGHT_TO_DIR,
	                   tr("Attach current highlight setting to current directory of programs"));
	setActionText(ActionId::DE_ATTACH_HIGHLIGHT_TO_DIR, tr("Remove highlight settings from the directory"));
	setActionExToolTip(ActionId::DE_ATTACH_HIGHLIGHT_TO_DIR, tr("Remove highlight settings from the directory"));
}

void GUI::WorkShop::StatusBarActions::loadIcons()
{
	setActionIcon(ActionId::ATTACH_HIGHLIGHT_TO_DIR, QIcon(":/images/attach.png"));
	setActionIcon(ActionId::DE_ATTACH_HIGHLIGHT_TO_DIR, QIcon(":/images/deattach.png"));
}


QAction* GUI::WorkShop::StatusBarActions::attachToDir()
{
	return action(ActionId::ATTACH_HIGHLIGHT_TO_DIR);
}

QAction* GUI::WorkShop::StatusBarActions::deAttachToDir()
{
	return action(ActionId::DE_ATTACH_HIGHLIGHT_TO_DIR);
}
