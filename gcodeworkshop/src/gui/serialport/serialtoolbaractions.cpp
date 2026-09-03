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

#include "serialtoolbaractions.h"


GUI::SerialToolBarActions::SerialToolBarActions(GCodeWorkShop* parent) : Actions::ActionKit(parent)
{
	makeAction(GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::CLOSE);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::RECEIVE);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::SEND);
	makeAction(GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG);

	connect(parent, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(parent, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));

	SerialToolBarActions::loadIcons();
	SerialToolBarActions::loadTranslations();
}

GUI::SerialToolBarActions::~SerialToolBarActions()
{
}

void GUI::SerialToolBarActions::loadTranslations()
{
	setActionText(GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR,
	              tr("Attach current port settings to current directory of programs"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR,
	                   tr("Attach current port settings to current directory of programs"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR,
	              tr("Remove settings from the current directory"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR,
	                   tr("Remove settings from the current directory"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::CLOSE, tr("Close send/receive toolbar"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::CLOSE, tr("Close send/receive toolbar"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER, tr("Start application \"GCodeFileServer\""));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER,
	                   tr("Start application \"GCodeFileServer\""));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG, tr("Serial port configuration"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG, tr("Serial port configuration"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::RECEIVE, tr("Receive new file"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::RECEIVE, tr("Receive new file"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::SEND, tr("Send current file"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::SEND, tr("Send current file"));
	setActionText(GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG, tr("Check serial port settings"));
	setActionExToolTip(GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG, tr("Check serial port settings"));
}

void GUI::SerialToolBarActions::loadIcons()
{
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR, QIcon(":/images/attach.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR, QIcon(":/images/deattach.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER, QIcon(":/images/spserver.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG, QIcon(":/images/serialconfig.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::RECEIVE, QIcon(":/images/receive.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::SEND, QIcon(":/images/send.png"));
	setActionIcon(GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG, QIcon(":/images/serialtest.png"));
}

QAction* GUI::SerialToolBarActions::configPort()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG);
}

QAction* GUI::SerialToolBarActions::receive()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::RECEIVE);
}

QAction* GUI::SerialToolBarActions::send()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::SEND);
}

QAction* GUI::SerialToolBarActions::attachToDir()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR);
}

QAction* GUI::SerialToolBarActions::deAttachToDir()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR);
}

QAction* GUI::SerialToolBarActions::testPort()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG);
}

QAction* GUI::SerialToolBarActions::close()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::CLOSE);
}

QAction* GUI::SerialToolBarActions::runFileServer()
{
	return action(GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER);
}
