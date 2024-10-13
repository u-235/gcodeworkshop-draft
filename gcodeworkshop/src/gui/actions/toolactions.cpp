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

#include <gui/actions/actionkit.h>      // for ActionKit
#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <gui/actions/knownactions.h>   // for KnownActions

#include "toolactions.h"    // for ToolActions


using ActionId = GUI::Actions::KnownActions::Tool;


GUI::Actions::ToolActions::ToolActions(GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CALC), &QAction::triggered, app, &GCodeWorkShop::doCalc);
	connect(makeAction(ActionId::DIFF), &QAction::triggered, app, &GCodeWorkShop::doDiff);
	connect(makeAction(ActionId::DIFF_EDITOR), &QAction::triggered, app, &GCodeWorkShop::diffEditorFile);
	connect(makeAction(ActionId::DIFF_LEFT), &QAction::triggered, app, &GCodeWorkShop::doDiffL);
	connect(makeAction(ActionId::DIFF_RIGHT), &QAction::triggered, app, &GCodeWorkShop::doDiffR);
	connect(makeAction(ActionId::INLINE_CALC), &QAction::triggered, app, &GCodeWorkShop::doShowInLineCalc);
	connect(makeAction(ActionId::SHOW_SERIAL_TOOLBAR), &QAction::triggered, app, &GCodeWorkShop::createSerialToolBar);

	connect(app, &GCodeWorkShop::updateIcons, this, &GUI::Actions::ToolActions::loadIcons);
	connect(app, &GCodeWorkShop::updateTranslations, this, &GUI::Actions::ToolActions::loadTranslations);
	connect(app, &GCodeWorkShop::updateShortcuts, this, &GUI::Actions::ToolActions::loadShortcuts);

	diff()->setCheckable(true);
	showSerialToolBar()->setCheckable(true);

	GUI::Actions::ToolActions::loadIcons();
	GUI::Actions::ToolActions::loadTranslations();
}

GUI::Actions::ToolActions::~ToolActions()
{
}

void GUI::Actions::ToolActions::loadTranslations()
{
	setActionText(ActionId::CALC, tr("Calculator"));
	setActionExToolTip(ActionId::CALC, tr("Run calculator"));
	setActionText(ActionId::DIFF, tr("On/off diff window"));
	setActionExToolTip(ActionId::DIFF, tr("Show diff window"));
	setActionText(ActionId::DIFF_EDITOR, tr("Show unsaved changes"));
	setActionExToolTip(ActionId::DIFF_EDITOR, tr("Show diff of currently edited file and file on disk"));
	setActionText(ActionId::DIFF_LEFT, tr("Show diff - open current file in left diff window"));
	setActionExToolTip(ActionId::DIFF_LEFT, tr("Show diff - open current file in left diff window"));
	setActionText(ActionId::DIFF_RIGHT, tr("Show diff - open current file in right diff window"));
	setActionExToolTip(ActionId::DIFF_RIGHT, tr("Show diff - open current file in right diff window"));
	setActionText(ActionId::INLINE_CALC, tr("Inline calculator"));
	setActionExToolTip(ActionId::INLINE_CALC, tr("Inline calculator"));
	setActionText(ActionId::SHOW_SERIAL_TOOLBAR, tr("Serial port send/receive"));
	setActionExToolTip(ActionId::SHOW_SERIAL_TOOLBAR, tr("Serial port send/receive"));
}

void GUI::Actions::ToolActions::loadIcons()
{
	setActionIcon(ActionId::CALC, QIcon(":/images/calc.png"));
	setActionIcon(ActionId::DIFF, QIcon(":/images/diff.png"));
	setActionIcon(ActionId::DIFF_EDITOR, QIcon(":/images/diff_editor.png"));
	setActionIcon(ActionId::DIFF_LEFT, QIcon(":/images/diffl.png"));
	setActionIcon(ActionId::DIFF_RIGHT, QIcon(":/images/diffr.png"));
	setActionIcon(ActionId::INLINE_CALC, QIcon(":/images/inlinecalc.png"));
	setActionIcon(ActionId::SHOW_SERIAL_TOOLBAR, QIcon(":/images/serial.png"));
}

QAction* GUI::Actions::ToolActions::calc()
{
	return action(ActionId::CALC);
}

QAction* GUI::Actions::ToolActions::diff()
{
	return action(ActionId::DIFF);
}

QAction* GUI::Actions::ToolActions::diffEditor()
{
	return action(ActionId::DIFF_EDITOR);
}

QAction* GUI::Actions::ToolActions::diffLeft()
{
	return action(ActionId::DIFF_LEFT);
}

QAction* GUI::Actions::ToolActions::diffRight()
{
	return action(ActionId::DIFF_RIGHT);
}

QAction* GUI::Actions::ToolActions::inLineCalc()
{
	return action(ActionId::INLINE_CALC);
}

QAction* GUI::Actions::ToolActions::showSerialToolBar()
{
	return action(ActionId::SHOW_SERIAL_TOOLBAR);
}
