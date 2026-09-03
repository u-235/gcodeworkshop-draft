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

#include <QIcon>            // for QIcon
#include <QLatin1String>    // for QLatin1String

#include <gui/actions/actionkit.h>      // for ActionKit
#include <gui/workshop/knownactions.h>  // for GUI::Actions::KnownActions

#include "serialtoolbaractions.h"


using ActionId = GUI::WorkShop::KnownActions::SerialToolBar;


GUI::SerialToolBarActions::SerialToolBarActions(QObject* parent) : Actions::ActionKit(parent)
{
	makeAction(ActionId::ATTACH_TO_DIR);
	makeAction(ActionId::DE_ATTACH_TO_DIR);
	makeAction(ActionId::CLOSE);
	makeAction(ActionId::RUN_FILESERVER);
	makeAction(ActionId::CONFIG_DIALOG);
	makeAction(ActionId::RECEIVE);
	makeAction(ActionId::SEND);
	makeAction(ActionId::TEST_DIALOG);

	SerialToolBarActions::loadIcons();
	SerialToolBarActions::loadTranslations();
}

GUI::SerialToolBarActions::~SerialToolBarActions()
{
}

void GUI::SerialToolBarActions::loadTranslations()
{
	setActionText(ActionId::ATTACH_TO_DIR,
	              tr("Attach current port settings to current directory of programs"));
	setActionExToolTip(ActionId::ATTACH_TO_DIR,
	                   tr("Attach current port settings to current directory of programs"));
	setActionText(ActionId::DE_ATTACH_TO_DIR,
	              tr("Remove settings from the current directory"));
	setActionExToolTip(ActionId::DE_ATTACH_TO_DIR,
	                   tr("Remove settings from the current directory"));
	setActionText(ActionId::CLOSE, tr("Close send/receive toolbar"));
	setActionExToolTip(ActionId::CLOSE, tr("Close send/receive toolbar"));
	setActionText(ActionId::RUN_FILESERVER, tr("Start application \"GCodeFileServer\""));
	setActionExToolTip(ActionId::RUN_FILESERVER,
	                   tr("Start application \"GCodeFileServer\""));
	setActionText(ActionId::CONFIG_DIALOG, tr("Serial port configuration"));
	setActionExToolTip(ActionId::CONFIG_DIALOG, tr("Serial port configuration"));
	setActionText(ActionId::RECEIVE, tr("Receive new file"));
	setActionExToolTip(ActionId::RECEIVE, tr("Receive new file"));
	setActionText(ActionId::SEND, tr("Send current file"));
	setActionExToolTip(ActionId::SEND, tr("Send current file"));
	setActionText(ActionId::TEST_DIALOG, tr("Check serial port settings"));
	setActionExToolTip(ActionId::TEST_DIALOG, tr("Check serial port settings"));
}

void GUI::SerialToolBarActions::loadIcons()
{
	setActionIcon(ActionId::ATTACH_TO_DIR, QIcon(":/images/attach.png"));
	setActionIcon(ActionId::DE_ATTACH_TO_DIR, QIcon(":/images/deattach.png"));
	setActionIcon(ActionId::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(ActionId::RUN_FILESERVER, QIcon(":/images/spserver.png"));
	setActionIcon(ActionId::CONFIG_DIALOG, QIcon(":/images/serialconfig.png"));
	setActionIcon(ActionId::RECEIVE, QIcon(":/images/receive.png"));
	setActionIcon(ActionId::SEND, QIcon(":/images/send.png"));
	setActionIcon(ActionId::TEST_DIALOG, QIcon(":/images/serialtest.png"));
}

QAction* GUI::SerialToolBarActions::configPort()
{
	return action(ActionId::CONFIG_DIALOG);
}

QAction* GUI::SerialToolBarActions::receive()
{
	return action(ActionId::RECEIVE);
}

QAction* GUI::SerialToolBarActions::send()
{
	return action(ActionId::SEND);
}

QAction* GUI::SerialToolBarActions::attachToDir()
{
	return action(ActionId::ATTACH_TO_DIR);
}

QAction* GUI::SerialToolBarActions::deAttachToDir()
{
	return action(ActionId::DE_ATTACH_TO_DIR);
}

QAction* GUI::SerialToolBarActions::testPort()
{
	return action(ActionId::TEST_DIALOG);
}

QAction* GUI::SerialToolBarActions::close()
{
	return action(ActionId::CLOSE);
}

QAction* GUI::SerialToolBarActions::runFileServer()
{
	return action(ActionId::RUN_FILESERVER);
}
