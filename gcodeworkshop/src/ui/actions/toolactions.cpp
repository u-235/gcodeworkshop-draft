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

#include "toolactions.h"    // for ToolActions


using ActionId = Ui::Actions::KnownActions::Tool;


Ui::Actions::ToolActions::ToolActions(::GCodeWorkShop* app) : ActionKit(app)
{
	connect(makeAction(ActionId::CALC), SIGNAL(triggered()), app, SLOT(doCalc()));
	connect(makeAction(ActionId::DIFF), SIGNAL(triggered()), app, SLOT(doDiff()));
	connect(makeAction(ActionId::DIFF_EDITOR), SIGNAL(triggered()), app, SLOT(diffEditorFile()));
	connect(makeAction(ActionId::DIFF_LEFT), SIGNAL(triggered()), app, SLOT(doDiffL()));
	connect(makeAction(ActionId::DIFF_RIGHT), SIGNAL(triggered()), app, SLOT(doDiffR()));
	connect(makeAction(ActionId::INLINE_CALC), SIGNAL(triggered()), app, SLOT(doShowInLineCalc()));
	connect(makeAction(ActionId::SHOW_SERIAL_TOOLBAR), SIGNAL(triggered()), app, SLOT(createSerialToolBar()));

	connect(app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(app, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	diff()->setCheckable(true);
	showSerialToolBar()->setCheckable(true);

	Ui::Actions::ToolActions::loadIcons();
	Ui::Actions::ToolActions::loadTranslations();
}

Ui::Actions::ToolActions::~ToolActions()
{
}

void Ui::Actions::ToolActions::loadTranslations()
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

void Ui::Actions::ToolActions::loadIcons()
{
	setActionIcon(ActionId::CALC, QIcon(":/images/calc.png"));
	setActionIcon(ActionId::DIFF, QIcon(":/images/diff.png"));
	setActionIcon(ActionId::DIFF_EDITOR, QIcon(":/images/diff_editor.png"));
	setActionIcon(ActionId::DIFF_LEFT, QIcon(":/images/diffl.png"));
	setActionIcon(ActionId::DIFF_RIGHT, QIcon(":/images/diffr.png"));
	setActionIcon(ActionId::INLINE_CALC, QIcon(":/images/inlinecalc.png"));
	setActionIcon(ActionId::SHOW_SERIAL_TOOLBAR, QIcon(":/images/serial.png"));
}

QAction* Ui::Actions::ToolActions::calc()
{
	return action(ActionId::CALC);
}

QAction* Ui::Actions::ToolActions::diff()
{
	return action(ActionId::DIFF);
}

QAction* Ui::Actions::ToolActions::diffEditor()
{
	return action(ActionId::DIFF_EDITOR);
}

QAction* Ui::Actions::ToolActions::diffLeft()
{
	return action(ActionId::DIFF_LEFT);
}

QAction* Ui::Actions::ToolActions::diffRight()
{
	return action(ActionId::DIFF_RIGHT);
}

QAction* Ui::Actions::ToolActions::inLineCalc()
{
	return action(ActionId::INLINE_CALC);
}

QAction* Ui::Actions::ToolActions::showSerialToolBar()
{
	return action(ActionId::SHOW_SERIAL_TOOLBAR);
}
