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
#include <QApplication>     // for QApplication, qApp
#include <QIcon>            // for QIcon
#include <QLatin1String>    // for QLatin1String

#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <ui/actions/actionkit.h>       // for ActionKit
#include <ui/actions/knownactions.h>    // for KnownActions

#include "helpactions.h"    // for HelpActions


using ActionId = Ui::Actions::KnownActions::Help;


Ui::Actions::HelpActions::HelpActions(::GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::ABOUT), SIGNAL(triggered()), app, SLOT(about()));
	connect(makeAction(ActionId::ABOUT_QT), SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(makeAction(ActionId::CREATE_GLOBAL_TOOL_TIP), SIGNAL(triggered()), app, SLOT(createGlobalToolTipsFile()));
	connect(makeAction(ActionId::CREATE_USER_TOOL_TIP), SIGNAL(triggered()), app, SLOT(createUserToolTipsFile()));

	connect(app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(app, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	Ui::Actions::HelpActions::loadIcons();
	Ui::Actions::HelpActions::loadTranslations();
}

Ui::Actions::HelpActions::~HelpActions()
{
}

void Ui::Actions::HelpActions::loadTranslations()
{
	setActionText(ActionId::ABOUT, tr("&About"));
	setActionExToolTip(ActionId::ABOUT, tr("Show the application's About box"));
	setActionText(ActionId::ABOUT_QT, tr("About &Qt"));
	setActionExToolTip(ActionId::ABOUT_QT, tr("Show the Qt library's About box"));
	setActionText(ActionId::CREATE_GLOBAL_TOOL_TIP, tr("&Create global cnc tooltips"));
	setActionExToolTip(ActionId::CREATE_GLOBAL_TOOL_TIP, tr("Create default global cnc tooltips file"));
	setActionText(ActionId::CREATE_USER_TOOL_TIP, tr("&Create user cnc tooltips"));
	setActionExToolTip(ActionId::CREATE_USER_TOOL_TIP, tr("Create/edit user cnc tooltips file"));
}

void Ui::Actions::HelpActions::loadIcons()
{
	setActionIcon(ActionId::ABOUT, QIcon(":/images/about.png"));
	setActionIcon(ActionId::ABOUT_QT, QIcon(":/images/about_qt.png"));
	setActionIcon(ActionId::CREATE_GLOBAL_TOOL_TIP, QIcon(":/images/tooltips_glob.png"));
	setActionIcon(ActionId::CREATE_USER_TOOL_TIP, QIcon(":/images/tooltips_user.png"));
}

QAction* Ui::Actions::HelpActions::about()
{
	return action(ActionId::ABOUT);
}

QAction* Ui::Actions::HelpActions::aboutQt()
{
	return action(ActionId::ABOUT_QT);
}

QAction* Ui::Actions::HelpActions::createGlobalToolTips()
{
	return action(ActionId::CREATE_GLOBAL_TOOL_TIP);
}

QAction* Ui::Actions::HelpActions::createUserToolTips()
{
	return action(ActionId::CREATE_USER_TOOL_TIP);
}
