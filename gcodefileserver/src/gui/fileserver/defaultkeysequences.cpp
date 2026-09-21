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
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QKeySequence>     // for QKeySequence
#include <QString>          // for QString, operator<

#include "gui/fileserver/defaultkeysequences.h"
#include "gui/fileserver/knownactions.h"


QMap<QString, QKeySequence> GUI::FileServer::defaultKeySequence()
{
	return QMap<QString, QKeySequence> {
		{GUI::FileServer::KnownActions::Window::ABOUT, QKeySequence()}
	};
}
