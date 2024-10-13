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

#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <ui/actions/actionkit.h>       // for ActionKit
#include <ui/actions/knownactions.h>    // for KnownActions

#include "windowactions.h"  // for WindowActions


using ActionId = Ui::Actions::KnownActions::Window;


Ui::Actions::WindowActions::WindowActions(::GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CASCADE), SIGNAL(triggered()), app, SLOT(cascadeSubWindows()));
	connect(makeAction(ActionId::NEXT), SIGNAL(triggered()), app, SLOT(activateNextSubWindow()));
	connect(makeAction(ActionId::PREVIOUS), SIGNAL(triggered()), app, SLOT(activatePreviousSubWindow()));
	makeAction(ActionId::SEPARATOR)->setSeparator(true);
	connect(makeAction(ActionId::TILE_HORIZ), SIGNAL(triggered()), app, SLOT(tileSubWindowsHorizontally()));
	connect(makeAction(ActionId::TILE_VERT), SIGNAL(triggered()), app, SLOT(tileSubWindowsVertycally()));

	connect(app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(app, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	Ui::Actions::WindowActions::loadIcons();
	Ui::Actions::WindowActions::loadTranslations();
}

Ui::Actions::WindowActions::~WindowActions()
{
}

void Ui::Actions::WindowActions::loadTranslations()
{
	setActionText(ActionId::CASCADE, tr("&Cascade"));
	setActionExToolTip(ActionId::CASCADE, tr("Cascade the windows"));
	setActionText(ActionId::NEXT, tr("Ne&xt"));
	setActionExToolTip(ActionId::NEXT, tr("Move the focus to the next window"));
	setActionText(ActionId::PREVIOUS, tr("Pre&vious"));
	setActionExToolTip(ActionId::PREVIOUS, tr("Move the focus to the previous window"));
	setActionText(ActionId::TILE_HORIZ, tr("Tile &horyzontally"));
	setActionExToolTip(ActionId::TILE_HORIZ, tr("Tile the windows horyzontallyTile the windows horyzontally"));
	setActionText(ActionId::TILE_VERT, tr("Tile &vertycally"));
	setActionExToolTip(ActionId::TILE_VERT, tr("Tile the windows vertycally"));
}

void Ui::Actions::WindowActions::loadIcons()
{
	setActionIcon(ActionId::CASCADE, QIcon(":/images/cascade.png"));
	setActionIcon(ActionId::NEXT, QIcon(":/images/go-next.png"));
	setActionIcon(ActionId::PREVIOUS, QIcon(":/images/go-previous.png"));
	setActionIcon(ActionId::TILE_HORIZ, QIcon(":/images/tile_h.png"));
	setActionIcon(ActionId::TILE_VERT, QIcon(":/images/tile_v.png"));
}

QAction* Ui::Actions::WindowActions::cascade()
{
	return action(ActionId::CASCADE);
}

QAction* Ui::Actions::WindowActions::next()
{
	return action(ActionId::NEXT);
}

QAction* Ui::Actions::WindowActions::previous()
{
	return action(ActionId::PREVIOUS);
}

QAction* Ui::Actions::WindowActions::separator()
{
	return action(ActionId::SEPARATOR);
}

QAction* Ui::Actions::WindowActions::tileHoriz()
{
	return action(ActionId::TILE_HORIZ);
}

QAction* Ui::Actions::WindowActions::tileVert()

{
	return action(ActionId::TILE_VERT);
}
