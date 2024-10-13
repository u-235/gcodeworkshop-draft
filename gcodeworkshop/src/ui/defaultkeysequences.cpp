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

#include <QCoreApplication> // for translate
#include <QKeySequence>     // for QKeySequence
#include <QString>          // for QString, operator<

#include <ui/actions/knownactions.h>

#include "ui/defaultkeysequences.h"


#define tr(s) QCoreApplication::translate("QShortcut", s)


QMap<QString, QKeySequence> Ui::defaultKeySequence()
{
	return QMap<QString, QKeySequence> {
		{Ui::Actions::KnownActions::Addons::BHC, QKeySequence(tr("F8"))},
		{Ui::Actions::KnownActions::Addons::BLOCK_SKIP_DECREMENT, QKeySequence(tr("Ctrl+1"))},
		{Ui::Actions::KnownActions::Addons::BLOCK_SKIP_INCREMENT, QKeySequence(tr("Ctrl+2"))},
		{Ui::Actions::KnownActions::Addons::BLOCK_SKIP_REMOVE, QKeySequence(tr("Ctrl+3"))},
		{Ui::Actions::KnownActions::Addons::CHAMFER, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::CLEANUP, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::COMMENT_PARA, QKeySequence(tr("Ctrl+9"))},
		{Ui::Actions::KnownActions::Addons::COMMENT_SEMI, QKeySequence(tr("Ctrl+;"))},
		{Ui::Actions::KnownActions::Addons::COMPILE_MACRO, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::DOT, QKeySequence(tr("F6"))},
		{Ui::Actions::KnownActions::Addons::EMPTY_LINE_INSERT, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::EMPTY_LINE_REMOVE, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::FEEDS, QKeySequence(tr("F9"))},
		{Ui::Actions::KnownActions::Addons::I2M, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::I2MPROG, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::RENUMBER, QKeySequence(tr("F7"))},
		{Ui::Actions::KnownActions::Addons::SPACES_INSERT, QKeySequence(tr("F4"))},
		{Ui::Actions::KnownActions::Addons::SPACES_REMOVE, QKeySequence(tr("F5"))},
		{Ui::Actions::KnownActions::Addons::SWAP_AXES, QKeySequence()},
		{Ui::Actions::KnownActions::Addons::TRANGLE, QKeySequence()},

		{Ui::Actions::KnownActions::Edit::CONFIG, QKeySequence(QKeySequence::Preferences)},
		{Ui::Actions::KnownActions::Edit::COPY, QKeySequence(QKeySequence::Copy)},
		{Ui::Actions::KnownActions::Edit::CUT, QKeySequence(QKeySequence::Cut)},
		{Ui::Actions::KnownActions::Edit::DELETE, QKeySequence(QKeySequence::Delete)},
		{Ui::Actions::KnownActions::Edit::FIND, QKeySequence(QKeySequence::Find)},
		{Ui::Actions::KnownActions::Edit::PASTE, QKeySequence(QKeySequence::Paste)},
		{Ui::Actions::KnownActions::Edit::READ_ONLY, QKeySequence(tr("F12"))},
		{Ui::Actions::KnownActions::Edit::REDO, QKeySequence(QKeySequence::Redo)},
		{Ui::Actions::KnownActions::Edit::REPLACE, QKeySequence(tr("Ctrl+R"))},
		{Ui::Actions::KnownActions::Edit::SELECT_ALL, QKeySequence(QKeySequence::SelectAll)},
		{Ui::Actions::KnownActions::Edit::UNDO, QKeySequence(QKeySequence::Undo)},

		{Ui::Actions::KnownActions::File::CLOSE, QKeySequence()},
		{Ui::Actions::KnownActions::File::CLOSE_ALL, QKeySequence()}, // tr("Ctrl+")
		{Ui::Actions::KnownActions::File::EXIT, QKeySequence(tr("Ctrl+Q"))},
		{Ui::Actions::KnownActions::File::FIND_IN_FILES, QKeySequence()},
		{Ui::Actions::KnownActions::File::NEW, QKeySequence(QKeySequence::New)},
		{Ui::Actions::KnownActions::File::OPEN, QKeySequence(QKeySequence::Open)},
		{Ui::Actions::KnownActions::File::OPEN_EXAMPLE, QKeySequence()},
		{Ui::Actions::KnownActions::File::PRINT, QKeySequence(QKeySequence::Print)},
		{Ui::Actions::KnownActions::File::PRINT_PREVIEW, QKeySequence()},
		{Ui::Actions::KnownActions::File::SAVE, QKeySequence(QKeySequence::Save)},
		{Ui::Actions::KnownActions::File::SAVE_ALL, tr("Ctrl+Shift+S")},
		{Ui::Actions::KnownActions::File::SAVE_AS, QKeySequence(QKeySequence::SaveAs)}, // TODO replace with Ctrl+Alt+S ?
		{Ui::Actions::KnownActions::File::SESSION_MANAGER, QKeySequence()},

		{Ui::Actions::KnownActions::FindToolBar::FIND_NEXT, QKeySequence(QKeySequence::FindNext)},
		{Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS, QKeySequence(QKeySequence::FindPrevious)},
		{Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT, QKeySequence()},
		{Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS, QKeySequence()},
		{Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL, QKeySequence()},
		{Ui::Actions::KnownActions::FindToolBar::CLOSE, QKeySequence()},

		{Ui::Actions::KnownActions::Help::ABOUT, QKeySequence()},
		{Ui::Actions::KnownActions::Help::ABOUT_QT, QKeySequence()},
		{Ui::Actions::KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, QKeySequence()},
		{Ui::Actions::KnownActions::Help::CREATE_USER_TOOL_TIP, QKeySequence()},

		{Ui::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::CLOSE, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::COMM_APP, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::RECEIVE, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::SEND, QKeySequence()},
		{Ui::Actions::KnownActions::SerialToolBar::TEST_DIALOG, QKeySequence()},

		{Ui::Actions::KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},
		{Ui::Actions::KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},

		{Ui::Actions::KnownActions::Tool::CALC, QKeySequence()},
		{Ui::Actions::KnownActions::Tool::DIFF, QKeySequence()},
		{Ui::Actions::KnownActions::Tool::DIFF_EDITOR, QKeySequence()},
		{Ui::Actions::KnownActions::Tool::DIFF_LEFT, QKeySequence()},
		{Ui::Actions::KnownActions::Tool::DIFF_RIGHT, QKeySequence()},
		{Ui::Actions::KnownActions::Tool::INLINE_CALC, QKeySequence(tr("Ctrl+0"))},
		{Ui::Actions::KnownActions::Tool::SHOW_SERIAL_TOOLBAR, QKeySequence()},

		{Ui::Actions::KnownActions::Window::CASCADE, QKeySequence()},
		{Ui::Actions::KnownActions::Window::NEXT, QKeySequence(QKeySequence::Forward)},
		{Ui::Actions::KnownActions::Window::PREVIOUS, QKeySequence(QKeySequence::Back)},
		{Ui::Actions::KnownActions::Window::SEPARATOR, QKeySequence()},
		{Ui::Actions::KnownActions::Window::TILE_HORIZ, QKeySequence()},
		{Ui::Actions::KnownActions::Window::TILE_VERT, QKeySequence()}
	};
}
