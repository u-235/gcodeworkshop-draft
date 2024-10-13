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

#include <QLatin1String>    // for QLatin1String

#include <ui/actions/knownactions.h>    // IWYU pragma: associated


const QLatin1String Ui::Actions::KnownActions::Addons::BHC("Addons.Bhc");
const QLatin1String Ui::Actions::KnownActions::Addons::BLOCK_SKIP_DECREMENT("Addons.BlockSkipDecrement");
const QLatin1String Ui::Actions::KnownActions::Addons::BLOCK_SKIP_INCREMENT("Addons.BlockSkipIncrement");
const QLatin1String Ui::Actions::KnownActions::Addons::BLOCK_SKIP_REMOVE("Addons.BlockSkipRemove");
const QLatin1String Ui::Actions::KnownActions::Addons::CHAMFER("Addons.Chamfer");
const QLatin1String Ui::Actions::KnownActions::Addons::CLEANUP("Addons.Cleanup");
const QLatin1String Ui::Actions::KnownActions::Addons::COMMENT_PARA("Addons.CommentPara");
const QLatin1String Ui::Actions::KnownActions::Addons::COMMENT_SEMI("Addons.CommentSemi");
const QLatin1String Ui::Actions::KnownActions::Addons::COMPILE_MACRO("Addons.CompileMacro");
const QLatin1String Ui::Actions::KnownActions::Addons::DOT("Addons.Dot");
const QLatin1String Ui::Actions::KnownActions::Addons::EMPTY_LINE_INSERT("Addons.EmptyLineInsert");
const QLatin1String Ui::Actions::KnownActions::Addons::EMPTY_LINE_REMOVE("Addons.EmptyLineRemove");
const QLatin1String Ui::Actions::KnownActions::Addons::FEEDS("Addons.Feeds");
const QLatin1String Ui::Actions::KnownActions::Addons::I2M("Addons.I2m");
const QLatin1String Ui::Actions::KnownActions::Addons::I2MPROG("Addons.I2mProg");
const QLatin1String Ui::Actions::KnownActions::Addons::RENUMBER("Addons.Renumber");
const QLatin1String Ui::Actions::KnownActions::Addons::SPACES_INSERT("Addons.SpaceInsert");
const QLatin1String Ui::Actions::KnownActions::Addons::SPACES_REMOVE("Addons.SpaceRemove");
const QLatin1String Ui::Actions::KnownActions::Addons::SPLIT_PROGRAMMS("Addons.SplitProgramms");
const QLatin1String Ui::Actions::KnownActions::Addons::SWAP_AXES("Addons.SwapAxes");
const QLatin1String Ui::Actions::KnownActions::Addons::TRANGLE("Addons.Triangle");

const QLatin1String Ui::Actions::KnownActions::Edit::CONFIG("Edit.Config");
const QLatin1String Ui::Actions::KnownActions::Edit::COPY("Edit.Copy");
const QLatin1String Ui::Actions::KnownActions::Edit::CUT("Edit.Cut");
const QLatin1String Ui::Actions::KnownActions::Edit::DELETE("Edit.Delete");
const QLatin1String Ui::Actions::KnownActions::Edit::FIND("Edit.Find");
const QLatin1String Ui::Actions::KnownActions::Edit::PASTE("Edit.Paste");
const QLatin1String Ui::Actions::KnownActions::Edit::READ_ONLY("Edit.ReadOnly");
const QLatin1String Ui::Actions::KnownActions::Edit::REDO("Edit.Redo");
const QLatin1String Ui::Actions::KnownActions::Edit::REPLACE("Edit.Replace");
const QLatin1String Ui::Actions::KnownActions::Edit::SELECT_ALL("Edit.SelectAll");
const QLatin1String Ui::Actions::KnownActions::Edit::UNDO("Edit.Undo");

const QLatin1String Ui::Actions::KnownActions::File::CLOSE("File.Close");
const QLatin1String Ui::Actions::KnownActions::File::CLOSE_ALL("File.CloseAll");
const QLatin1String Ui::Actions::KnownActions::File::EXIT("File.Exit");
const QLatin1String Ui::Actions::KnownActions::File::FIND_IN_FILES("File.FindInFiles");
const QLatin1String Ui::Actions::KnownActions::File::NEW("File.New");
const QLatin1String Ui::Actions::KnownActions::File::OPEN("File.Open");
const QLatin1String Ui::Actions::KnownActions::File::OPEN_EXAMPLE("File.OpenExample");
const QLatin1String Ui::Actions::KnownActions::File::PRINT("File.Print");
const QLatin1String Ui::Actions::KnownActions::File::PRINT_PREVIEW("File.PrintPreview");
const QLatin1String Ui::Actions::KnownActions::File::SAVE("File.Save");
const QLatin1String Ui::Actions::KnownActions::File::SAVE_ALL("File.SaveAll");
const QLatin1String Ui::Actions::KnownActions::File::SAVE_AS("File.SaveAs");
const QLatin1String Ui::Actions::KnownActions::File::SESSION_MANAGER("File.SessionManager");

const QLatin1String Ui::Actions::KnownActions::FindToolBar::FIND_NEXT("FindToolBar.FindNext");
const QLatin1String Ui::Actions::KnownActions::FindToolBar::FIND_PREVIOUS("FindToolBar.FindPrevious");
const QLatin1String Ui::Actions::KnownActions::FindToolBar::REPLACE_NEXT("FindToolBar.ReplaceNext");
const QLatin1String Ui::Actions::KnownActions::FindToolBar::REPLACE_PREVIOUS("FindToolBar.ReplacePrevious");
const QLatin1String Ui::Actions::KnownActions::FindToolBar::REPLACE_ALL("FindToolBar.ReplaceAll");
const QLatin1String Ui::Actions::KnownActions::FindToolBar::CLOSE("FindToolBar.Close");

const QLatin1String Ui::Actions::KnownActions::Help::ABOUT("Help.About");
const QLatin1String Ui::Actions::KnownActions::Help::ABOUT_QT("Help.AboutQt");
const QLatin1String Ui::Actions::KnownActions::Help::CREATE_GLOBAL_TOOL_TIP("Help.CreateGlobalToolTip");
const QLatin1String Ui::Actions::KnownActions::Help::CREATE_USER_TOOL_TIP("Help.CreateUserToolTip");

const QLatin1String Ui::Actions::KnownActions::SerialToolBar::ATTACH_TO_DIR("SerialToolBar.AttachToDir");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::CLOSE("SerialToolBar.Close");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::COMM_APP("SerialToolBar.CommApp");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::CONFIG_DIALOG("SerialToolBar.ConfigDialog");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::DE_ATTACH_TO_DIR("SerialToolBar.DeAttachToDir");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::RECEIVE("SerialToolBar.Receve");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::SEND("SerialToolBar.Send");
const QLatin1String Ui::Actions::KnownActions::SerialToolBar::TEST_DIALOG("SerialToolBar.TestDialog");

const QLatin1String Ui::Actions::KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR("StatusBar.AttachHightToDir");
const QLatin1String Ui::Actions::KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR("StatusBar.DeAttachHightToDir");

const QLatin1String Ui::Actions::KnownActions::Tool::CALC("Tool.Calc");
const QLatin1String Ui::Actions::KnownActions::Tool::DIFF("Tool.Diff");
const QLatin1String Ui::Actions::KnownActions::Tool::DIFF_EDITOR("Tool.DiffEditor");
const QLatin1String Ui::Actions::KnownActions::Tool::DIFF_LEFT("Tool.DiffLeft");
const QLatin1String Ui::Actions::KnownActions::Tool::DIFF_RIGHT("Tool.DiffRight");
const QLatin1String Ui::Actions::KnownActions::Tool::INLINE_CALC("Tool.InlineCalc");
const QLatin1String Ui::Actions::KnownActions::Tool::SHOW_SERIAL_TOOLBAR("Tool.ShowSerialToolBar");

const QLatin1String Ui::Actions::KnownActions::Window::CASCADE("Window.Cascade");
const QLatin1String Ui::Actions::KnownActions::Window::NEXT("Window.Next");
const QLatin1String Ui::Actions::KnownActions::Window::PREVIOUS("Window.Previous");
const QLatin1String Ui::Actions::KnownActions::Window::SEPARATOR("Window.Separator");
const QLatin1String Ui::Actions::KnownActions::Window::TILE_HORIZ("Window.TileHoriz");
const QLatin1String Ui::Actions::KnownActions::Window::TILE_VERT("Window.TileVert");
