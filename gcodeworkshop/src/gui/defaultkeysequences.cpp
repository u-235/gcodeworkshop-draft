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

#include <utility>  // for pair

#include <QCoreApplication> // for translate
#include <QKeySequence>     // for QKeySequence
#include <QString>          // for QString, operator<

#include "gui/defaultkeysequences.h"
#include "gui/actions/knownactions.h"


#define tr(s) QCoreApplication::translate("QShortcut", s)


QMap<QString, QKeySequence> GUI::defaultKeySequence()
{
	return QMap<QString, QKeySequence> {
		{GUI::Actions::KnownActions::Addons::BHC, QKeySequence(tr("F8"))},
		{GUI::Actions::KnownActions::Addons::BLOCK_SKIP_DECREMENT, QKeySequence(tr("Ctrl+1"))},
		{GUI::Actions::KnownActions::Addons::BLOCK_SKIP_INCREMENT, QKeySequence(tr("Ctrl+2"))},
		{GUI::Actions::KnownActions::Addons::BLOCK_SKIP_REMOVE, QKeySequence(tr("Ctrl+3"))},
		{GUI::Actions::KnownActions::Addons::CHAMFER, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::CLEANUP, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::COMMENT_PARA, QKeySequence(tr("Ctrl+9"))},
		{GUI::Actions::KnownActions::Addons::COMMENT_SEMI, QKeySequence(tr("Ctrl+;"))},
		{GUI::Actions::KnownActions::Addons::COMPILE_MACRO, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::DOT, QKeySequence(tr("F6"))},
		{GUI::Actions::KnownActions::Addons::EMPTY_LINE_INSERT, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::EMPTY_LINE_REMOVE, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::FEEDS, QKeySequence(tr("F9"))},
		{GUI::Actions::KnownActions::Addons::I2M, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::I2MPROG, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::RENUMBER, QKeySequence(tr("F7"))},
		{GUI::Actions::KnownActions::Addons::SPACES_INSERT, QKeySequence(tr("F4"))},
		{GUI::Actions::KnownActions::Addons::SPACES_REMOVE, QKeySequence(tr("F5"))},
		{GUI::Actions::KnownActions::Addons::SWAP_AXES, QKeySequence()},
		{GUI::Actions::KnownActions::Addons::TRIANGLE, QKeySequence()},

		{GUI::Actions::KnownActions::Edit::CONFIG, QKeySequence(QKeySequence::Preferences)},
		{GUI::Actions::KnownActions::Edit::COPY, QKeySequence(QKeySequence::Copy)},
		{GUI::Actions::KnownActions::Edit::CUT, QKeySequence(QKeySequence::Cut)},
		{GUI::Actions::KnownActions::Edit::DELETE, QKeySequence(QKeySequence::Delete)},
		{GUI::Actions::KnownActions::Edit::FIND, QKeySequence(QKeySequence::Find)},
		{GUI::Actions::KnownActions::Edit::PASTE, QKeySequence(QKeySequence::Paste)},
		{GUI::Actions::KnownActions::Edit::READ_ONLY, QKeySequence(tr("F12"))},
		{GUI::Actions::KnownActions::Edit::REDO, QKeySequence(QKeySequence::Redo)},
		{GUI::Actions::KnownActions::Edit::REPLACE, QKeySequence(tr("Ctrl+R"))},
		{GUI::Actions::KnownActions::Edit::SELECT_ALL, QKeySequence(QKeySequence::SelectAll)},
		{GUI::Actions::KnownActions::Edit::UNDO, QKeySequence(QKeySequence::Undo)},

		{GUI::Actions::KnownActions::File::CLOSE, QKeySequence()},
		{GUI::Actions::KnownActions::File::CLOSE_ALL, QKeySequence()}, // tr("Ctrl+")
		{GUI::Actions::KnownActions::File::EXIT, QKeySequence(tr("Ctrl+Q"))},
		{GUI::Actions::KnownActions::File::FIND_IN_FILES, QKeySequence()},
		{GUI::Actions::KnownActions::File::NEW, QKeySequence(QKeySequence::New)},
		{GUI::Actions::KnownActions::File::OPEN, QKeySequence(QKeySequence::Open)},
		{GUI::Actions::KnownActions::File::OPEN_EXAMPLE, QKeySequence()},
		{GUI::Actions::KnownActions::File::PRINT, QKeySequence(QKeySequence::Print)},
		{GUI::Actions::KnownActions::File::PRINT_PREVIEW, QKeySequence()},
		{GUI::Actions::KnownActions::File::SAVE, QKeySequence(QKeySequence::Save)},
		{GUI::Actions::KnownActions::File::SAVE_ALL, tr("Ctrl+Shift+S")},
		{GUI::Actions::KnownActions::File::SAVE_AS, QKeySequence(QKeySequence::SaveAs)}, // TODO replace with Ctrl+Alt+S ?
		{GUI::Actions::KnownActions::File::SESSION_MANAGER, QKeySequence()},

		{GUI::Actions::KnownActions::FindToolBar::FIND_NEXT, QKeySequence(QKeySequence::FindNext)},
		{GUI::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, QKeySequence(QKeySequence::FindPrevious)},
		{GUI::Actions::KnownActions::FindToolBar::REPLACE_NEXT, QKeySequence()},
		{GUI::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, QKeySequence()},
		{GUI::Actions::KnownActions::FindToolBar::REPLACE_ALL, QKeySequence()},
		{GUI::Actions::KnownActions::FindToolBar::CLOSE, QKeySequence()},

		{GUI::Actions::KnownActions::Help::ABOUT, QKeySequence()},
		{GUI::Actions::KnownActions::Help::ABOUT_QT, QKeySequence()},
		{GUI::Actions::KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, QKeySequence()},
		{GUI::Actions::KnownActions::Help::CREATE_USER_TOOL_TIP, QKeySequence()},

		{GUI::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::CLOSE, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::RUN_FILESERVER, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::RECEIVE, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::SEND, QKeySequence()},
		{GUI::Actions::KnownActions::SerialToolBar::TEST_DIALOG, QKeySequence()},

		{GUI::Actions::KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},
		{GUI::Actions::KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},

		{GUI::Actions::KnownActions::Tool::CALC, QKeySequence()},
		{GUI::Actions::KnownActions::Tool::DIFF, QKeySequence()},
		{GUI::Actions::KnownActions::Tool::DIFF_EDITOR, QKeySequence()},
		{GUI::Actions::KnownActions::Tool::DIFF_LEFT, QKeySequence()},
		{GUI::Actions::KnownActions::Tool::DIFF_RIGHT, QKeySequence()},
		{GUI::Actions::KnownActions::Tool::INLINE_CALC, QKeySequence(tr("Ctrl+0"))},
		{GUI::Actions::KnownActions::Tool::SHOW_SERIAL_TOOLBAR, QKeySequence()},

		{GUI::Actions::KnownActions::Window::CASCADE, QKeySequence()},
		{GUI::Actions::KnownActions::Window::NEXT, QKeySequence(QKeySequence::Forward)},
		{GUI::Actions::KnownActions::Window::PREVIOUS, QKeySequence(QKeySequence::Back)},
		{GUI::Actions::KnownActions::Window::SEPARATOR, QKeySequence()},
		{GUI::Actions::KnownActions::Window::TILE_HORIZ, QKeySequence()},
		{GUI::Actions::KnownActions::Window::TILE_VERT, QKeySequence()}
	};
}
