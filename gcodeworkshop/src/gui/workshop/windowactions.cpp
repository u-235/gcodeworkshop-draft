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
#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/workshop/knownactions.h>  // for KnownActions

#include "windowactions.h"  // for WindowActions


using ActionId = GUI::WorkShop::KnownActions::Window;


GUI::WorkShop::WindowActions::WindowActions(GCodeWorkShop* app, QObject* parent) :
	ActionKit(parent)
{
	connect(makeAction(ActionId::CASCADE), &QAction::triggered, app, &GCodeWorkShop::cascadeSubWindows);
	connect(makeAction(ActionId::NEXT), &QAction::triggered, app, &GCodeWorkShop::activateNextSubWindow);
	connect(makeAction(ActionId::PREVIOUS), &QAction::triggered, app, &GCodeWorkShop::activatePreviousSubWindow);
	makeAction(ActionId::SEPARATOR)->setSeparator(true);
	connect(makeAction(ActionId::TILE_HORIZ), &QAction::triggered, app, &GCodeWorkShop::tileSubWindowsHorizontally);
	connect(makeAction(ActionId::TILE_VERT), &QAction::triggered, app, &GCodeWorkShop::tileSubWindowsVertycally);

	connect(app, &GCodeWorkShop::updateIcons, this, &GUI::WorkShop::WindowActions::loadIcons);
	connect(app, &GCodeWorkShop::updateTranslations, this, &GUI::WorkShop::WindowActions::loadTranslations);
	connect(app, &GCodeWorkShop::updateShortcuts, this, & GUI::WorkShop::WindowActions::loadShortcuts);

	GUI::WorkShop::WindowActions::loadIcons();
	GUI::WorkShop::WindowActions::loadTranslations();
}

GUI::WorkShop::WindowActions::~WindowActions()
{
}

void GUI::WorkShop::WindowActions::loadTranslations()
{
	setActionText(ActionId::CASCADE, tr("&Cascade"));
	setActionExToolTip(ActionId::CASCADE, tr("Cascade the windows"));
	setActionText(ActionId::NEXT, tr("Ne&xt"));
	setActionExToolTip(ActionId::NEXT, tr("Move the focus to the next window"));
	setActionText(ActionId::PREVIOUS, tr("Pre&vious"));
	setActionExToolTip(ActionId::PREVIOUS, tr("Move the focus to the previous window"));
	setActionText(ActionId::TILE_HORIZ, tr("Tile &horizontally"));
	setActionExToolTip(ActionId::TILE_HORIZ, tr("Tile the windows horizontally"));
	setActionText(ActionId::TILE_VERT, tr("Tile &vertycally"));
	setActionExToolTip(ActionId::TILE_VERT, tr("Tile the windows vertycally"));
}

void GUI::WorkShop::WindowActions::loadIcons()
{
	setActionIcon(ActionId::CASCADE, QIcon(":/images/cascade.png"));
	setActionIcon(ActionId::NEXT, QIcon(":/images/go-next.png"));
	setActionIcon(ActionId::PREVIOUS, QIcon(":/images/go-previous.png"));
	setActionIcon(ActionId::TILE_HORIZ, QIcon(":/images/tile_h.png"));
	setActionIcon(ActionId::TILE_VERT, QIcon(":/images/tile_v.png"));
}

QAction* GUI::WorkShop::WindowActions::cascade()
{
	return action(ActionId::CASCADE);
}

QAction* GUI::WorkShop::WindowActions::next()
{
	return action(ActionId::NEXT);
}

QAction* GUI::WorkShop::WindowActions::previous()
{
	return action(ActionId::PREVIOUS);
}

QAction* GUI::WorkShop::WindowActions::separator()
{
	return action(ActionId::SEPARATOR);
}

QAction* GUI::WorkShop::WindowActions::tileHoriz()
{
	return action(ActionId::TILE_HORIZ);
}

QAction* GUI::WorkShop::WindowActions::tileVert()

{
	return action(ActionId::TILE_VERT);
}
