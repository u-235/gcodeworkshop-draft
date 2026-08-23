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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>              // for QByteArray
#include <QMainWindow>             // for QMainWindow
#include <QObject>                 // for slots, Q_OBJECT, signals
#include <QPoint>                  // for QPoint
#include <QPointer>                // for QPointer
#include <QSize>                   // for QSize
#include <QString>                 // for QString
#include <QStringList>             // for QStringList

class QAction;
class QClipboard;
class QComboBox;
class QLabel;
class QMenu;
class QSettings;
class QToolBar;
class QToolButton;
class QWidget;

#include <utils/keysequencemap.h>   // for KeySequenceMap

class GCodeWorkShop;
class KDiff3App;


namespace GUI {
class ClipboardPanel;
class FileBrowserPanel;
class FileTablePanel;
class FindInFilesPanel;
class FindToolBar;
class ProjectPanel;

namespace Actions {
class EditActions;
class FileActions;
class HelpActions;
class ToolActions;
class WindowActions;
} // namespace Actions
} // namespace GUI

namespace Addons {
class Actions;
}

namespace Ui {
class MainWindow;
}


namespace GUI {
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(GCodeWorkShop* application, QWidget* parent = nullptr);
	~MainWindow();

	GCodeWorkShop* app();
	bool maybeSave();

	Addons::Actions* addonsActions();
	GUI::Actions::EditActions* editActions();
	GUI::Actions::FileActions* fileActions();
	GUI::Actions::HelpActions* helpActions();
	GUI::Actions::ToolActions* toolActions();
	GUI::Actions::WindowActions* windowActions();

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
	bool MdiTabbedMode();
public slots:
	void setMdiTabbedMode(bool tabbed);
	void closeCurrentWindow();
	void closeAllMdiWindows();
	void tileSubWindowsHorizontally();
	void tileSubWindowsVertycally();
	void cascadeSubWindows();
	void activateNextSubWindow();
	void activatePreviousSubWindow();
protected:
	void createDiffApp();
public slots:
	void doDiffL();
	void doDiffR();
	void diffTwoFiles(const QString& filename1, const QString& filename2);
	void diffEditorFile();
	void doDiff();

	void createSerialToolBar();
	void closeSerialToolbar();
	void updateCurrentSerialConfig();
	void loadSerialConfignames();
	void serialConfig();
	void serialConfigTest();
	void sendButtonClicked();
	void receiveButtonClicked();
	void attachToDirButtonClicked(bool attach = true);
	void deAttachToDirButtonClicked();

	void showFindReplaceToolBar(bool replace);
	void showFindToolBar();
	void showReplaceToolBar();

	void setHighLightMode(int mode);

protected slots:
	void changeSession(QAction* action);

protected:
	void closeEvent(QCloseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void moveEvent(QMoveEvent* event) override;
	/*
	 * To implement the opening of dragged files, we override the event handlers.
	 * see textedit.h and textedit.cpp
	 * see https://doc.qt.io/qt-5/dnd.html#dropping
	 */
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
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

	bool m_MdiTabbedMode;
	bool m_findInFilesHighlightEnable;

	QClipboard* clipboard;
	KeySequenceMap m_shortcuts;
	Addons::Actions* m_addonsActions;
	GUI::Actions::EditActions* m_editActions;
	GUI::Actions::FileActions* m_fileActions;
	GUI::Actions::HelpActions* m_helpActions;
	GUI::Actions::ToolActions* m_toolActions;
	GUI::Actions::WindowActions* m_windowActions;

	GUI::FindInFilesPanel* findFiles;
	GUI::FindToolBar* m_findToolBar;
	GUI::ProjectPanel* m_projectPanel;

	bool panelHidden;
	QByteArray panelState;

	KDiff3App* diffApp;

	QMenu* fileMenu;
	QMenu* recentFileMenu;
	QMenu* editMenu;
	QMenu* toolsMenu;
	QMenu* blockSkipMenu;
	QMenu* windowMenu;
	QMenu* helpMenu;
	QMenu* sessionsMenu;

	QToolBar* fileToolBar;
	QToolBar* editToolBar;
	QToolBar* windowToolBar;
	QToolBar* toolsToolBar;

	QLabel* labelStat1;
	QToolButton* readOnlyButton;
	QComboBox* highlightTypeCombo;
	QAction* attachHighlightToDirAct;
	QToolButton* attachHighlightButton;
	QAction* deAttachHighlightToDirAct;
	QToolButton* deAttachHighlightButton;

	QPointer<QToolBar> serialToolBar;
	QAction* configPortAct;
	QAction* receiveAct;
	QAction* sendAct;
	QAction* attachToDirAct;
	QAction* deAttachToDirAct;
	QAction* diagAct;
	QAction* serialCloseAct;
	QAction* commAppAct;
	QComboBox* configBox;

private:
	GCodeWorkShop* m_app;
	Ui::MainWindow* ui;
};
} // namespace GUI

#endif // MAINWINDOW_H
