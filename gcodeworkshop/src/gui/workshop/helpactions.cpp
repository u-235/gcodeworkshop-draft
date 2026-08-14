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
#include <QKeySequence>     // for QTypeInfo<>::isComplex, QTypeInfo<>::isPointer
#include <QLatin1String>    // for QLatin1String
#include <QtGlobal>         // for QT_VERSION, QT_VERSION_CHECK

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	#include <QMap>
#endif

#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/workshop/knownactions.h>  // for KnownActions

#include "helpactions.h"    // for HelpActions


using ActionId = GUI::WorkShop::KnownActions::Help;


GUI::WorkShop::HelpActions::HelpActions(GCodeWorkShop* app, QObject* parent) :
	ActionKit(parent)
{
	connect(makeAction(ActionId::ABOUT), &QAction::triggered, app, &GCodeWorkShop::about);
	connect(makeAction(ActionId::ABOUT_QT), &QAction::triggered, qApp, &QApplication::aboutQt);
	connect(makeAction(ActionId::CREATE_GLOBAL_TOOL_TIP), &QAction::triggered, app,
	        &GCodeWorkShop::createGlobalToolTipsFile);
	connect(makeAction(ActionId::CREATE_USER_TOOL_TIP), &QAction::triggered, app, &GCodeWorkShop::createUserToolTipsFile);

	connect(app, &GCodeWorkShop::updateIcons, this, &GUI::WorkShop::HelpActions::loadIcons);
	connect(app, &GCodeWorkShop::updateTranslations, this, &GUI::WorkShop::HelpActions::loadTranslations);
	connect(app, &GCodeWorkShop::updateShortcuts, this, &GUI::WorkShop::HelpActions::loadShortcuts);

	GUI::WorkShop::HelpActions::loadIcons();
	GUI::WorkShop::HelpActions::loadTranslations();
}

GUI::WorkShop::HelpActions::~HelpActions()
{
}

void GUI::WorkShop::HelpActions::loadTranslations()
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

void GUI::WorkShop::HelpActions::loadIcons()
{
	setActionIcon(ActionId::ABOUT, QIcon(":/images/about.png"));
	setActionIcon(ActionId::ABOUT_QT, QIcon(":/images/about_qt.png"));
	setActionIcon(ActionId::CREATE_GLOBAL_TOOL_TIP, QIcon(":/images/tooltips_glob.png"));
	setActionIcon(ActionId::CREATE_USER_TOOL_TIP, QIcon(":/images/tooltips_user.png"));
}

QAction* GUI::WorkShop::HelpActions::about()
{
	return action(ActionId::ABOUT);
}

QAction* GUI::WorkShop::HelpActions::aboutQt()
{
	return action(ActionId::ABOUT_QT);
}

QAction* GUI::WorkShop::HelpActions::createGlobalToolTips()
{
	return action(ActionId::CREATE_GLOBAL_TOOL_TIP);
}

QAction* GUI::WorkShop::HelpActions::createUserToolTips()
{
	return action(ActionId::CREATE_USER_TOOL_TIP);
}
