/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
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

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QByteArray>              // for QByteArray
#include <QMainWindow>             // for QMainWindow
#include <QObject>                 // for slots, Q_OBJECT, signals
#include <QPoint>                  // for QPoint
#include <QSize>                   // for QSize
#include <QString>                 // for QString
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	template<typename T> class QList;
	using QStringList = QList<QString>;
#endif

class QAction;
class QClipboard;
class QComboBox;
class QLabel;
class QMdiArea;
class QMenu;
class QSettings;
class QToolBar;
class QWidget;

class GCodeWorkShop;
class KDiff3App;

namespace Addons {
class Actions;
}

namespace Ui {
class MainWindow;
}


namespace GUI {
class ClipboardPanel;
class FileBrowserPanel;
class FileTablePanel;
class FindInFilesPanel;
class FindToolBar;
class ProjectPanel;
class SerialToolBar;

namespace WorkShop {
class EditActions;
class FileActions;
class HelpActions;
class ToolActions;
class WindowActions;


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(GCodeWorkShop* application, QWidget* parent = nullptr);
	~MainWindow();

	GCodeWorkShop* app();
	bool maybeSave();

	Addons::Actions* addonsActions();
	GUI::WorkShop::EditActions* editActions();
	GUI::WorkShop::FileActions* fileActions();
	GUI::WorkShop::HelpActions* helpActions();
	GUI::WorkShop::ToolActions* toolActions();
	GUI::WorkShop::WindowActions* windowActions();

	QComboBox* highlightComboBox();

signals:
	void clipboardTextChanged(const QString& text);

public slots:
	void loadTranslations();
	void loadIcons();

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg) const;

	void updateStatusBar();
	void updateMenus();
	void updateWindowMenu();
	void updateRecentFilesMenu(const QStringList& fileList);
	void updateSessionMenus(const QStringList& sessionList);

	void hidePanel();
public:
	bool isPanelHidden();

public slots:
	void findInFiles();
	void clipboardChanged();
	void clipboardSetText(const QString& text);
public:
	QMdiArea* mdiArea() const;
	bool MdiTabbedMode();
public slots:
	void setMdiTabbedMode(bool tabbed);
protected:
	void createDiffApp();
public slots:
	void doDiffL();
	void doDiffR();
	void diffTwoFiles(const QString& filename1, const QString& filename2);
	void diffEditorFile();
	void doDiff();

	void createSerialToolBar();
	void updateCurrentSerialConfig();

	void showFindReplaceToolBar(bool replace);

	void setHighLightMode();

protected slots:
	void changeSession(QAction* action);

protected:
	void closeEvent(QCloseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void moveEvent(QMoveEvent* event) override;
	/*
	 * To implement the opening of dragged files, we override the event handlers.
	 * see src/gcodereventfilter.h and src/gcodereventfilter.cpp
	 * see https://doc.qt.io/qt-5/dnd.html#dropping
	 */
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

	void createMenus();
	void createToolBars();
	void setupStatusBar();
	void createFindToolBar();
	void setupToolTabs();
	GUI::FileBrowserPanel* createFileBrowserPanel();
	GUI::ProjectPanel* createProjectPanel();
	GUI::ClipboardPanel* createClipboardPanel();
	GUI::FileTablePanel* createFileTablePanel();

	struct MWConfig {
		QPoint pos;
		QSize size;
	} mMWConfig;

	GCodeWorkShop* m_app;
	Ui::MainWindow* ui;

	bool m_MdiTabbedMode;
	bool m_findInFilesHighlightEnable;

	QClipboard* m_clipboard;
	Addons::Actions* m_addonsActions;
	GUI::WorkShop::EditActions* m_editActions;
	GUI::WorkShop::FileActions* m_fileActions;
	GUI::WorkShop::HelpActions* m_helpActions;
	GUI::WorkShop::ToolActions* m_toolActions;
	GUI::WorkShop::WindowActions* m_windowActions;

	QToolBar* m_fileToolBar;
	QToolBar* m_editToolBar;
	QToolBar* m_windowToolBar;
	QToolBar* m_toolsToolBar;

	GUI::FindToolBar* m_findToolBar;
	GUI::SerialToolBar* m_serialToolBar;
	GUI::FindInFilesPanel* m_findInFiles;
	GUI::ProjectPanel* m_projectPanel;

	bool m_panelHidden;
	QByteArray m_panelState;

	KDiff3App* m_diffApp;

	QMenu* m_fileMenu;
	QMenu* m_recentFileMenu;
	QMenu* m_editMenu;
	QMenu* m_toolsMenu;
	QMenu* m_blockSkipMenu;
	QMenu* m_windowMenu;
	QMenu* m_helpMenu;
	QMenu* m_sessionsMenu;

	QLabel* m_statusBarLabel;
	QComboBox* m_statusBarHighlight;
};
} // namespace WorkShop
} // namespace GUI

#endif // GUI_MAINWINDOW_H
