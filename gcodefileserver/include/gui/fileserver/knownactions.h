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
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_FILESERVER_KNOWNACTIONS_H
#define GUI_FILESERVER_KNOWNACTIONS_H

class QLatin1String;


namespace GUI {
namespace FileServer {
namespace KnownActions {
struct Window {
	static const QLatin1String ABOUT;
	static const QLatin1String BROWSE_SAVE_FOLDER;
	static const QLatin1String CONFIGURE_PORT;
	static const QLatin1String MAXIMIZE;
	static const QLatin1String MINIMIZE;
	static const QLatin1String QUIT;
	static const QLatin1String RESTORE;
	static const QLatin1String SERVER_CLOSE;
	static const QLatin1String SERVER_RESET;
	static const QLatin1String SERVER_START;
	static const QLatin1String SERVER_STOP;
	static const QLatin1String SHOW_NEW_FILES;
	static const QLatin1String START_MINIMIZED;
};
} // namespace KnownActions
} // namespace FileServer
} // namespace GUI

#endif // GUI_FILESERVER_KNOWNACTIONS_H
