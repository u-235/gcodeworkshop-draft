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

#include "gui/workshop/defaultkeysequences.h"
#include "gui/workshop/knownactions.h"


QMap<QString, QKeySequence> GUI::WorkShop::defaultKeySequence()
{
	return QMap<QString, QKeySequence> {
		{GUI::WorkShop::KnownActions::Addons::BHC, QKeySequence("F8")},
		{GUI::WorkShop::KnownActions::Addons::BLOCK_SKIP_DECREMENT, QKeySequence("Ctrl+1")},
		{GUI::WorkShop::KnownActions::Addons::BLOCK_SKIP_INCREMENT, QKeySequence("Ctrl+2")},
		{GUI::WorkShop::KnownActions::Addons::BLOCK_SKIP_REMOVE, QKeySequence("Ctrl+3")},
		{GUI::WorkShop::KnownActions::Addons::CHAMFER, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::CLEANUP, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::COMMENT_PARA, QKeySequence("Ctrl+9")},
		{GUI::WorkShop::KnownActions::Addons::COMMENT_SEMI, QKeySequence("Ctrl+;")},
		{GUI::WorkShop::KnownActions::Addons::COMPILE_MACRO, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::DOT, QKeySequence("F6")},
		{GUI::WorkShop::KnownActions::Addons::EMPTY_LINE_INSERT, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::EMPTY_LINE_REMOVE, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::FEEDS, QKeySequence("F9")},
		{GUI::WorkShop::KnownActions::Addons::I2M, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::I2MPROG, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::RENUMBER, QKeySequence("F7")},
		{GUI::WorkShop::KnownActions::Addons::SPACES_INSERT, QKeySequence("F4")},
		{GUI::WorkShop::KnownActions::Addons::SPACES_REMOVE, QKeySequence("F5")},
		{GUI::WorkShop::KnownActions::Addons::SWAP_AXES, QKeySequence()},
		{GUI::WorkShop::KnownActions::Addons::TRIANGLE, QKeySequence()},

		{GUI::WorkShop::KnownActions::Edit::CONFIG, QKeySequence(QKeySequence::Preferences)},
		{GUI::WorkShop::KnownActions::Edit::COPY, QKeySequence(QKeySequence::Copy)},
		{GUI::WorkShop::KnownActions::Edit::CUT, QKeySequence(QKeySequence::Cut)},
		{GUI::WorkShop::KnownActions::Edit::DELETE, QKeySequence(QKeySequence::Delete)},
		{GUI::WorkShop::KnownActions::Edit::FIND, QKeySequence(QKeySequence::Find)},
		{GUI::WorkShop::KnownActions::Edit::PASTE, QKeySequence(QKeySequence::Paste)},
		{GUI::WorkShop::KnownActions::Edit::READ_ONLY, QKeySequence("F12")},
		{GUI::WorkShop::KnownActions::Edit::REDO, QKeySequence(QKeySequence::Redo)},
		{GUI::WorkShop::KnownActions::Edit::REPLACE, QKeySequence("Ctrl+R")},
		{GUI::WorkShop::KnownActions::Edit::SELECT_ALL, QKeySequence(QKeySequence::SelectAll)},
		{GUI::WorkShop::KnownActions::Edit::UNDO, QKeySequence(QKeySequence::Undo)},

		{GUI::WorkShop::KnownActions::File::CLOSE, QKeySequence()},
		{GUI::WorkShop::KnownActions::File::CLOSE_ALL, QKeySequence()}, // tr("Ctrl+")
		{GUI::WorkShop::KnownActions::File::EXIT, QKeySequence("Ctrl+Q")},
		{GUI::WorkShop::KnownActions::File::FIND_IN_FILES, QKeySequence()},
		{GUI::WorkShop::KnownActions::File::NEW, QKeySequence(QKeySequence::New)},
		{GUI::WorkShop::KnownActions::File::OPEN, QKeySequence(QKeySequence::Open)},
		{GUI::WorkShop::KnownActions::File::OPEN_EXAMPLE, QKeySequence()},
		{GUI::WorkShop::KnownActions::File::PRINT, QKeySequence(QKeySequence::Print)},
		{GUI::WorkShop::KnownActions::File::PRINT_PREVIEW, QKeySequence()},
		{GUI::WorkShop::KnownActions::File::SAVE, QKeySequence(QKeySequence::Save)},
		{GUI::WorkShop::KnownActions::File::SAVE_ALL, QKeySequence("Ctrl+Shift+S")},
		{GUI::WorkShop::KnownActions::File::SAVE_AS, QKeySequence(QKeySequence::SaveAs)}, // TODO replace with Ctrl+Alt+S ?
		{GUI::WorkShop::KnownActions::File::SESSION_MANAGER, QKeySequence()},

		{GUI::WorkShop::KnownActions::FindToolBar::FIND_NEXT, QKeySequence(QKeySequence::FindNext)},
		{GUI::WorkShop::KnownActions::FindToolBar::FIND_PREVIOUS, QKeySequence(QKeySequence::FindPrevious)},
		{GUI::WorkShop::KnownActions::FindToolBar::REPLACE_NEXT, QKeySequence()},
		{GUI::WorkShop::KnownActions::FindToolBar::REPLACE_PREVIOUS, QKeySequence()},
		{GUI::WorkShop::KnownActions::FindToolBar::REPLACE_ALL, QKeySequence()},
		{GUI::WorkShop::KnownActions::FindToolBar::CLOSE, QKeySequence()},

		{GUI::WorkShop::KnownActions::Help::ABOUT, QKeySequence()},
		{GUI::WorkShop::KnownActions::Help::ABOUT_QT, QKeySequence()},
		{GUI::WorkShop::KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, QKeySequence()},
		{GUI::WorkShop::KnownActions::Help::CREATE_USER_TOOL_TIP, QKeySequence()},

		{GUI::WorkShop::KnownActions::SerialToolBar::ATTACH_TO_DIR, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::CLOSE, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::RUN_FILESERVER, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::CONFIG_DIALOG, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::RECEIVE, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::SEND, QKeySequence()},
		{GUI::WorkShop::KnownActions::SerialToolBar::TEST_DIALOG, QKeySequence()},

		{GUI::WorkShop::KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},
		{GUI::WorkShop::KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},

		{GUI::WorkShop::KnownActions::Tool::CALC, QKeySequence()},
		{GUI::WorkShop::KnownActions::Tool::DIFF, QKeySequence()},
		{GUI::WorkShop::KnownActions::Tool::DIFF_EDITOR, QKeySequence()},
		{GUI::WorkShop::KnownActions::Tool::DIFF_LEFT, QKeySequence()},
		{GUI::WorkShop::KnownActions::Tool::DIFF_RIGHT, QKeySequence()},
		{GUI::WorkShop::KnownActions::Tool::INLINE_CALC, QKeySequence("Ctrl+0")},
		{GUI::WorkShop::KnownActions::Tool::SHOW_SERIAL_TOOLBAR, QKeySequence()},

		{GUI::WorkShop::KnownActions::Window::CASCADE, QKeySequence()},
		{GUI::WorkShop::KnownActions::Window::NEXT, QKeySequence(QKeySequence::Forward)},
		{GUI::WorkShop::KnownActions::Window::PREVIOUS, QKeySequence(QKeySequence::Back)},
		{GUI::WorkShop::KnownActions::Window::SEPARATOR, QKeySequence()},
		{GUI::WorkShop::KnownActions::Window::TILE_HORIZ, QKeySequence()},
		{GUI::WorkShop::KnownActions::Window::TILE_VERT, QKeySequence()}
	};
}
