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

#ifndef GUI_ACTIONS_KNOWNACTIONS_H
#define GUI_ACTIONS_KNOWNACTIONS_H

class QLatin1String;


namespace GUI {
namespace Actions {
namespace KnownActions {
struct Addons {
	static const QLatin1String BHC;
	static const QLatin1String BLOCK_SKIP_DECREMENT;
	static const QLatin1String BLOCK_SKIP_INCREMENT;
	static const QLatin1String BLOCK_SKIP_REMOVE;
	static const QLatin1String CHAMFER;
	static const QLatin1String CLEANUP;
	static const QLatin1String COMMENT_PARA;
	static const QLatin1String COMMENT_SEMI;
	static const QLatin1String COMPILE_MACRO;
	static const QLatin1String DOT;
	static const QLatin1String EMPTY_LINE_INSERT;
	static const QLatin1String EMPTY_LINE_REMOVE;
	static const QLatin1String FEEDS;
	static const QLatin1String I2M;
	static const QLatin1String I2MPROG;
	static const QLatin1String RENUMBER;
	static const QLatin1String SPACES_INSERT;
	static const QLatin1String SPACES_REMOVE;
	static const QLatin1String SPLIT_PROGRAMMS;
	static const QLatin1String SWAP_AXES;
	static const QLatin1String TRIANGLE;
};

struct Edit {
	static const QLatin1String CONFIG;
	static const QLatin1String COPY;
	static const QLatin1String CUT;
	static const QLatin1String DELETE;
	static const QLatin1String FIND;
	static const QLatin1String PASTE;
	static const QLatin1String READ_ONLY;
	static const QLatin1String REDO;
	static const QLatin1String REPLACE;
	static const QLatin1String SELECT_ALL;
	static const QLatin1String UNDO;
};

struct File {
	static const QLatin1String CLOSE;
	static const QLatin1String CLOSE_ALL;
	static const QLatin1String EXIT;
	static const QLatin1String FIND_IN_FILES;
	static const QLatin1String NEW;
	static const QLatin1String OPEN;
	static const QLatin1String OPEN_EXAMPLE;
	static const QLatin1String PRINT;
	static const QLatin1String PRINT_PREVIEW;
	static const QLatin1String SAVE;
	static const QLatin1String SAVE_ALL;
	static const QLatin1String SAVE_AS;
	static const QLatin1String SESSION_MANAGER;
};

struct FindToolBar {
	static const QLatin1String FIND_NEXT;
	static const QLatin1String FIND_PREVIOUS;
	static const QLatin1String REPLACE_NEXT;
	static const QLatin1String REPLACE_PREVIOUS;
	static const QLatin1String REPLACE_ALL;
	static const QLatin1String CLOSE;
};

struct Help {
	static const QLatin1String ABOUT;
	static const QLatin1String ABOUT_QT;
	static const QLatin1String CREATE_GLOBAL_TOOL_TIP;
	static const QLatin1String CREATE_USER_TOOL_TIP;
};

struct SerialToolBar {
	static const QLatin1String ATTACH_TO_DIR;
	static const QLatin1String CLOSE;
	static const QLatin1String RUN_FILESERVER;
	static const QLatin1String CONFIG_DIALOG;
	static const QLatin1String DE_ATTACH_TO_DIR;
	static const QLatin1String RECEIVE;
	static const QLatin1String SEND;
	static const QLatin1String TEST_DIALOG;
};

struct StatusBar {
	static const QLatin1String ATTACH_HIGHLIGHT_TO_DIR;
	static const QLatin1String DE_ATTACH_HIGHLIGHT_TO_DIR;
};

struct Tool {
	static const QLatin1String CALC;
	static const QLatin1String DIFF;
	static const QLatin1String DIFF_EDITOR;
	static const QLatin1String DIFF_LEFT;
	static const QLatin1String DIFF_RIGHT;
	static const QLatin1String INLINE_CALC;
	static const QLatin1String SHOW_SERIAL_TOOLBAR;
};

struct Window {
	static const QLatin1String TILE_HORIZ;
	static const QLatin1String TILE_VERT;
	static const QLatin1String CASCADE;
	static const QLatin1String NEXT;
	static const QLatin1String PREVIOUS;
	static const QLatin1String SEPARATOR;
};
} // namespace KnownActions
} // namespace Actions
} // namespace GUI

#endif // GUI_ACTIONS_KNOWNACTIONS_H
