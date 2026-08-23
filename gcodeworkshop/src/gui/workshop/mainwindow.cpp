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

#include <QAction>                             // for QAction
#include <QActionGroup>                        // for QActionGroup
#include <QApplication>                        // for QApplication
#include <QByteArray>                          // for QByteArray
#include <QClipboard>                          // for QClipboard
#include <QCloseEvent>                         // for QCloseEvent
#include <QComboBox>                           // for QComboBox
#include <QDir>                                // for QDir, operator|
#include <QDragEnterEvent>                     // for QDragEnterEvent
#include <QDropEvent>                          // for QDropEvent
#include <QFile>                               // for QFile
#include <QFileInfo>                           // for QFileInfo, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QFileI...
#include <QFrame>                              // for QFrame
#include <QIODevice>                           // for QIODevice
#include <QIcon>                               // for QIcon
#include <QLabel>                              // for QLabel
#include <QLatin1String>                       // for QLatin1String
#include <QList>                               // for QList
#include <QMainWindow>                         // for QMainWindow
#include <QMdiArea>                            // for QMdiArea
#include <QMdiSubWindow>                       // for QMdiSubWindow
#include <QMenu>                               // for QMenu
#include <QMenuBar>                            // for QMenuBar
#include <QMessageBox>                         // for QMessageBox
#include <QMimeData>                           // for QMimeData
#include <QObject>                             // for SIGNAL, SLOT, emit
#include <QPoint>                              // for QPoint
#include <QPushButton>                         // for QPushButton
#include <QRect>                               // for QRect
#include <QResizeEvent>                        // for QResizeEvent
#include <QSettings>                           // for QSettings
#include <QSize>                               // for QSize
#include <QSplitter>                           // for QSplitter
#include <QStatusBar>                          // for QStatusBar
#include <QString>                             // for QString, operator+
#include <QTabBar>                             // for QTabBar
#include <QTabWidget>                          // for QTabWidget
#include <QToolBar>                            // for QToolBar
#include <QToolButton>                         // for QToolButton
#include <QUrl>                                // for QUrl, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic
#include <QVariant>                            // for QVariant
#include <QWidget>                             // for QWidget
#include <Qt>                                  // for WindowState, ToolBarArea, CursorShape, WidgetAttribute, FocusR...
#include <QtGlobal>                            // for QTypeInfo<>::isComplex, QTypeInfo<>::isLarge, QTypeInfo<>::isS...

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	using QStringList = QList<QString>;
#endif

#include <gcodeworkshop.h>
#include <addons-actions.h>             // for Actions
#include <document.h>                   // for Document
#include <documentmanager.h>            // for DocumentManager
#include <documentstyle.h>              // for DocumentStyle
#include <documentwidgetproperties.h>   // for DocumentWidgetProperties
#include <gcoderdocument.h>             // for GCoderDocument
#include <gcoderstyle.h>                // for GCoderStyle, HighlightColors
#include <gcoderwidgetproperties.h>     // for GCoderWidgetProperties
#include <kdiff3/kdiff3.h>              // for KDiff3App
#include <utils/keysequencemap.h>       // for KeySequenceMap
#include <utils/medium.h>               // for Medium

#include "gcoder.h"                             // for DOCUMENT_TYPE
#include "gui/clipboard/clipboardpanel.h"       // for ClipboardPanel
#include "gui/filebrowser/filebrowserpanel.h"   // for FileBrowserPanel
#include "gui/filetable/filetablepanel.h"       // for FileTablePanel
#include "gui/findinfiles/findinfilespanel.h"   // for FindInFilesPanel
#include "gui/findtoolbar/findtoolbar.h"        // for FindToolBar
#include "gui/project/projectpanel.h"           // for ProjectPanel
#include "gui/serialport/serialtoolbar.h"       // for SerialToolBar
#include "gui/workshop/editactions.h"           // for EditActions
#include "gui/workshop/fileactions.h"           // for FileActions
#include "gui/workshop/helpactions.h"           // for HelpActions
#include "gui/workshop/statusbaractions.h"      // for StatusBarActions
#include "gui/workshop/toolactions.h"           // for ToolActions
#include "gui/workshop/windowactions.h"         // for WindowActions
#include "highlightmode.h"                      // for HighlightMode

#include "gui/workshop/mainwindow.h"    // IWYU pragma: associated
#include "ui_mainwindow.h"


GUI::WorkShop::MainWindow::MainWindow(GCodeWorkShop* application, QWidget* parent)
	: QMainWindow{parent},
	  m_app(application),
	  ui(new Ui::MainWindow()),
	  m_MdiTabbedMode(false),
	  m_findInFilesHighlightEnable(true),
	  m_clipboard(QApplication::clipboard()),
	  m_addonsActions(new Addons::Actions(application, this)),
	  m_editActions(new GUI::WorkShop::EditActions(application, this)),
	  m_fileActions(new GUI::WorkShop::FileActions(application, this)),
	  m_helpActions(new GUI::WorkShop::HelpActions(application, this)),
	  m_toolActions(new GUI::WorkShop::ToolActions(application, this)),
	  m_windowActions(new GUI::WorkShop::WindowActions(application, this)),
	  m_fileToolBar(nullptr),
	  m_editToolBar(nullptr),
	  m_windowToolBar(nullptr),
	  m_toolsToolBar(nullptr),
	  m_findToolBar(nullptr),
	  m_serialToolBar(nullptr),
	  m_findInFiles(nullptr),
	  m_projectPanel(nullptr),
	  m_panelHidden(false),
	  m_panelState(),
	  m_diffApp(nullptr),
	  m_fileMenu(nullptr),
	  m_recentFileMenu(nullptr),
	  m_editMenu(nullptr),
	  m_toolsMenu(nullptr),
	  m_blockSkipMenu(nullptr),
	  m_windowMenu(nullptr),
	  m_helpMenu(nullptr),
	  m_sessionsMenu(),
	  m_statusBarLabel(new QLabel("    ")),
	  m_statusBarHighlight(new QComboBox())
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setAcceptDrops(true);

	m_panelHidden = ui->vSplitter->isHidden();
	connect(ui->hideButton, &QPushButton::clicked, this, &MainWindow::hidePanel);

	createToolBars();
	setupStatusBar();
	setupToolTabs();

	connect(ui->mdiArea, &QMdiArea::subWindowActivated, app(), &GCodeWorkShop::activeWindowChanged);

	createFindToolBar();
	connect(app()->documentManager(), &DocumentManager::activeDocumentChanged, m_findToolBar, &FindToolBar::highlight);
	createMenus();
	updateWindowMenu();
	updateMenus();
	ui->vSplitter->addWidget(createFileTablePanel());

	setWindowTitle(tr("GCodeWorkShop"));
	setWindowIcon(QIcon(":/images/edytornc.png"));

	connect(m_clipboard, &QClipboard::dataChanged, this, &MainWindow::clipboardChanged);
}

GUI::WorkShop::MainWindow::~MainWindow()
{
	delete ui;
}

GCodeWorkShop* GUI::WorkShop::MainWindow::app()
{
	return m_app;
}

bool GUI::WorkShop::MainWindow::maybeSave()
{
	return m_projectPanel->maybeSave();
}

Addons::Actions* GUI::WorkShop::MainWindow::addonsActions()
{
	return m_addonsActions;
}

GUI::WorkShop::EditActions* GUI::WorkShop::MainWindow::editActions()
{
	return m_editActions;
}

GUI::WorkShop::FileActions* GUI::WorkShop::MainWindow::fileActions()
{
	return m_fileActions;
}

GUI::WorkShop::HelpActions* GUI::WorkShop::MainWindow::helpActions()
{
	return m_helpActions;
}

GUI::WorkShop::ToolActions* GUI::WorkShop::MainWindow::toolActions()
{
	return m_toolActions;
}

GUI::WorkShop::WindowActions* GUI::WorkShop::MainWindow::windowActions()
{
	return m_windowActions;
}

QComboBox* GUI::WorkShop::MainWindow::highlightComboBox()
{
	return m_statusBarHighlight;
}

void GUI::WorkShop::MainWindow::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::WorkShop::MainWindow::loadIcons()
{

}

void GUI::WorkShop::MainWindow::loadSettings(QSettings* cfg)
{
	cfg->beginGroup("mainwindow");
	mMWConfig.pos = cfg->value("pos", QPoint(0, 0)).toPoint();
	mMWConfig.size = cfg->value("size", QSize(400, 240)).toSize();
	bool maximized = cfg->value("maximized", false).toBool();
	QByteArray state = cfg->value("state", QByteArray()).toByteArray();
	cfg->endGroup();

	setGeometry(QRect(mMWConfig.pos, mMWConfig.size));
	setWindowState(maximized ? Qt::WindowMaximized : Qt::WindowNoState);

	if (cfg->value("SerialToolbarShown", false).toBool()) {
		createSerialToolBar();
		m_toolActions->showSerialToolBar()->setChecked(true);
	}

	m_findToolBar->loadSettings(cfg);

	if (cfg->value("FindToolBarShown", false).toBool()) {
		showFindReplaceToolBar(false);
	}

	restoreState(state);

	setMdiTabbedMode(cfg->value("TabbetMode", false).toBool());

	ui->vSplitter->restoreState(cfg->value("VSplitterState", QByteArray()).toByteArray());

	m_panelState = cfg->value("ProjectPanelState", QByteArray()).toByteArray();
	ui->hSplitter->restoreState(m_panelState);
	m_panelHidden = cfg->value("PanelHidden", false).toBool();

	if (m_panelHidden) {
		ui->vSplitter->hide();
		ui->frame->setMaximumWidth(ui->hideButton->width());
		ui->hideButton->setText(">>");
	}

	ui->tabWidget->setCurrentIndex(cfg->value("TabCurrentIndex", 0).toInt());
}

void GUI::WorkShop::MainWindow::saveSettings(QSettings* cfg) const
{
	cfg->beginGroup("mainwindow");
	cfg->setValue("pos", mMWConfig.pos);
	cfg->setValue("size", mMWConfig.size);
	cfg->setValue("maximized", isMaximized());
	cfg->setValue("state", saveState());
	cfg->endGroup();

	cfg->setValue("TabbetMode", m_MdiTabbedMode);

	cfg->setValue("SerialToolbarShown", (m_serialToolBar != nullptr));

	cfg->setValue("VSplitterState", ui->vSplitter->saveState());
	cfg->setValue("TabCurrentIndex", ui->tabWidget->currentIndex());

	if (m_panelHidden) {
		cfg->setValue("ProjectPanelState", m_panelState);
	} else {
		cfg->setValue("ProjectPanelState", ui->hSplitter->saveState());
	}

	cfg->setValue("PanelHidden", m_panelHidden);

	cfg->setValue("FindToolBarShown", !m_findToolBar->isHidden());
	m_findToolBar->saveSettings(cfg);
}

void GUI::WorkShop::MainWindow::updateStatusBar()
{
	GCoderDocument* gdoc = app()->activeGCoderDocument();

	if (gdoc) {
		int id = m_statusBarHighlight->findData(gdoc->highlightMode());
		m_statusBarHighlight->blockSignals(true);
		m_statusBarHighlight->setCurrentIndex(id);
		m_statusBarHighlight->blockSignals(false);

		int line = gdoc->currentLine();
		int column = gdoc->currentColumn();

		m_statusBarLabel->setText(tr(" Col: ") + QString::number(column + 1) +
		                          tr("  Line: ") + QString::number(line) +
		                          (gdoc->isModified() ? tr("  <b>Modified</b>  ") : " ") +
		                          (gdoc->isReadOnly() ? tr(" Read only  ") : " ") +
		                          (gdoc->overwriteMode() ? tr(" Overwrite  ") : tr(" Insert ")));

	}
}

void GUI::WorkShop::MainWindow::updateMenus()
{
	Document* doc = app()->activeDocument();
	GCoderDocument* gdoc = app()->activeGCoderDocument();
	bool hasMdiChildNotReadOnly;
	bool hasSelection;
	bool hasModifiedMdiChild;
	bool hasMdiChild = doc != nullptr;

	if (hasMdiChild) {
		hasMdiChildNotReadOnly = !doc->isReadOnly();
		hasSelection = gdoc ? gdoc->hasSelection() : false;
		hasModifiedMdiChild = doc->isModified();

		m_editActions->redo()->setEnabled(hasMdiChild && doc->isRedoAvailable());
		m_editActions->undo()->setEnabled(hasMdiChild && doc->isUndoAvailable());
	} else {
		hasMdiChildNotReadOnly = false;
		hasSelection = false;
		hasModifiedMdiChild = false;

		m_editActions->redo()->setEnabled(false);
		m_editActions->undo()->setEnabled(false);
	}

	m_fileActions->save()->setEnabled(hasModifiedMdiChild);
	m_fileActions->saveAll()->setEnabled(hasMdiChild);
	m_fileActions->saveAs()->setEnabled(hasMdiChild);
	m_fileActions->print()->setEnabled(hasMdiChild);
	m_fileActions->printPreview()->setEnabled(hasMdiChild);
	m_editActions->paste()->setEnabled(hasMdiChild);
	m_fileActions->close()->setEnabled(hasMdiChild);
	m_fileActions->closeAll()->setEnabled(hasMdiChild);

	windowActions()->tileHoriz()->setEnabled(hasMdiChild);
	windowActions()->tileVert()->setEnabled(hasMdiChild);
	windowActions()->cascade()->setEnabled(hasMdiChild);
	windowActions()->next()->setEnabled(hasMdiChild);
	windowActions()->previous()->setEnabled(hasMdiChild);
	windowActions()->separator()->setVisible(hasMdiChild);
	m_editActions->selectAll()->setEnabled(hasMdiChildNotReadOnly);
	m_editActions->find()->setEnabled(hasMdiChild);

	m_toolActions->diffLeft()->setEnabled(hasMdiChild);
	m_toolActions->diffRight()->setEnabled(hasMdiChild);
	m_toolActions->diffEditor()->setEnabled(hasMdiChildNotReadOnly);

	m_editActions->replace()->setEnabled(hasMdiChildNotReadOnly);
	m_editActions->readOnly()->setEnabled(hasMdiChild);
	m_addonsActions->renumber()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->dot()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->removeEmptyLines()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->insertEmptyLines()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->insertSpaces()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->removeSpaces()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->splitProgramms()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->i2mProg()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->compileMacro()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->cleanUp()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->swapAxes()->setEnabled(hasMdiChildNotReadOnly);
	m_addonsActions->paraComment()->setEnabled(hasMdiChildNotReadOnly && hasSelection);
	m_addonsActions->semiComment()->setEnabled(hasMdiChildNotReadOnly && hasSelection);
	m_addonsActions->blockSkipDecrement()->setEnabled(hasMdiChildNotReadOnly && hasSelection);
	m_addonsActions->blockSkipIncrement()->setEnabled(hasMdiChildNotReadOnly && hasSelection);
	m_addonsActions->blockSkipRemove()->setEnabled(hasMdiChildNotReadOnly && hasSelection);
	m_toolActions->inLineCalc()->setEnabled(hasMdiChild);

	if (!hasMdiChildNotReadOnly) {
		m_editActions->readOnly()->setChecked(true);
		m_editActions->readOnly()->setIcon(QIcon(":/images/lock.png"));
	} else {
		m_editActions->readOnly()->setChecked(false);
		m_editActions->readOnly()->setIcon(QIcon(":/images/unlock.png"));
	}

	m_editActions->cut()->setEnabled(hasSelection && hasMdiChildNotReadOnly);
	m_editActions->del()->setEnabled(hasSelection && hasMdiChildNotReadOnly);
	m_editActions->copy()->setEnabled(hasSelection);

	m_editActions->paste()->setEnabled((!m_clipboard->text().isEmpty()) && hasMdiChildNotReadOnly);

	if (doc) {
		m_fileActions->save()->setText(tr("&Save \"%1\"").arg(doc->fileName()));
		m_fileActions->saveAs()->setText(tr("Save \"%1\" &As...").arg(doc->fileName()));
		m_fileActions->close()->setText(tr("Cl&ose \"%1\"").arg(doc->fileName()));
	}

	updateStatusBar();
}

void GUI::WorkShop::MainWindow::updateWindowMenu()
{
	QString text;

	m_windowMenu->clear();
	m_windowMenu->addAction(m_fileActions->close());
	m_windowMenu->addAction(m_fileActions->closeAll());
	m_windowMenu->addSeparator();
	m_windowMenu->addAction(windowActions()->tileHoriz());
	m_windowMenu->addAction(windowActions()->tileVert());
	m_windowMenu->addAction(windowActions()->cascade());
	m_windowMenu->addSeparator();
	m_windowMenu->addAction(windowActions()->next());
	m_windowMenu->addAction(windowActions()->previous());
	m_windowMenu->addAction(windowActions()->separator());

	m_windowMenu->setAttribute(Qt::WA_AlwaysShowToolTips, true);

	QList<Document*> docList = app()->documentManager()->documentList();
	windowActions()->separator()->setVisible(!docList.isEmpty());

	for (int i = 0; i < docList.size(); ++i) {
		Document* doc = docList.at(i);

		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1).arg(doc->filePath());
		} else {
			text = tr("%1 %2").arg(i + 1).arg(doc->filePath());
		}

		QAction* action = m_windowMenu->addAction(text);
		action->setCheckable(true);
		action->setChecked(doc == app()->activeDocument());
		action->setToolTip(doc->brief());
		connect(action, &QAction::triggered, app(), [this, doc]() {
			app()->setActiveDocument(doc);
		});
	}
}

void GUI::WorkShop::MainWindow::updateRecentFilesMenu(const QStringList& fileList)
{
	m_recentFileMenu->clear();

	for (const QString& file : fileList) {
		QAction* newAc = m_recentFileMenu->addAction(QIcon(":/images/document-open-recent.png"), file);
		newAc->setData(file);
	}
}

void GUI::WorkShop::MainWindow::updateSessionMenus(const QStringList& sessionList)
{
	m_sessionsMenu->clear();

	QActionGroup* actionGroup = new QActionGroup(m_sessionsMenu);
	actionGroup->setExclusive(true);

	bool checked = true;

	for (const QString& name : sessionList) {
		QAction* action = actionGroup->addAction(name);
		action->setCheckable(true);
		action->setChecked(checked);
		checked = false;
	}

	m_sessionsMenu->addActions(actionGroup->actions());
}

void GUI::WorkShop::MainWindow::hidePanel()
{
	ui->hSplitter->setUpdatesEnabled(false);

	if (!m_panelHidden) {
		m_panelState = ui->hSplitter->saveState();
		ui->frame->setMaximumWidth(ui->hideButton->width());
		ui->vSplitter->hide();
		ui->hideButton->setText(">>");
		ui->hideButton->setToolTip(tr("Show"));
		m_panelHidden = true;
	} else {
		// FIXME
		//app()->fireCurrentDirChanged();
		ui->frame->setMaximumWidth(16777215);
		ui->vSplitter->show();
		ui->hideButton->setText("<<");
		ui->hideButton->setToolTip(tr("Hide"));
		ui->hSplitter->restoreState(m_panelState);
		m_panelHidden = false;
	}

	ui->hSplitter->updateGeometry();
	ui->hSplitter->setUpdatesEnabled(true);
}

bool GUI::WorkShop::MainWindow::isPanelHidden()
{
	return m_panelHidden;
}

void GUI::WorkShop::MainWindow::findInFiles()
{
	if (m_findInFiles == nullptr) {
		m_findInFiles = new GUI::FindInFilesPanel(ui->splitter);
		connect(app(), &GCodeWorkShop::intCapsLockChanged, m_findInFiles, &GUI::FindInFilesPanel::setCapsLockEnable);
		auto prop = dynamic_cast<GCoderWidgetProperties*>(app()->documentManager()->documentWidgetProperties(
		                GCoder::DOCUMENT_TYPE).get());

		if (prop) {
			m_findInFiles->setCapsLockEnable(prop->intCapsLock);
		}

		m_findInFiles->loadSettings(Medium::instance().settings());
		connect(m_findInFiles, &FindInFilesPanel::fileClicked, app(), &GCodeWorkShop::openFile);
		connect(app(), &GCodeWorkShop::loadSettings, m_findInFiles, &GUI::FindInFilesPanel::loadSettings);
		connect(app(), &GCodeWorkShop::saveSettings, m_findInFiles, &GUI::FindInFilesPanel::saveSettings);

		if (m_findInFilesHighlightEnable) {
			GCoderStyle* style = dynamic_cast<GCoderStyle*>(app()->documentManager()->documentStyle(GCoder::DOCUMENT_TYPE).get());

			if (style) {
				m_findInFiles->setHighlightColors(style->hColors);
			}
		}

		if (app()->activeDocument()) {
			m_findInFiles->setDir(app()->activeDocument()->path());
		}
	} else if (!m_fileActions->findFiles()->isChecked()) {
		m_findInFiles->saveSettings(Medium::instance().settings());
		m_findInFiles->close();
		m_findInFiles = nullptr;
	} else {
		m_findInFiles->show();
		m_fileActions->findFiles()->setChecked(true);
	}
}

void GUI::WorkShop::MainWindow::clipboardChanged()
{
	if (!isActiveWindow()) {
		return;
	}

	updateMenus();
	emit clipboardTextChanged(m_clipboard->text());
}

void GUI::WorkShop::MainWindow::clipboardSetText(const QString& text)
{
	if (!text.isEmpty()) {
		m_clipboard->setText(text);
	}
}

QMdiArea* GUI::WorkShop::MainWindow::mdiArea() const
{
	return ui->mdiArea;
}

bool GUI::WorkShop::MainWindow::MdiTabbedMode()
{
	return m_MdiTabbedMode;
}

void GUI::WorkShop::MainWindow::setMdiTabbedMode(bool tabbed)
{
	m_MdiTabbedMode = tabbed;

	if (m_MdiTabbedMode) {
		ui->mdiArea->setViewMode(QMdiArea::TabbedView);
		QTabBar* tab = ui->mdiArea->findChild<QTabBar*>();

		if (tab) {
			tab->setTabsClosable(true);
			// The tabs might be very wide
			tab->setExpanding(false);
		}
	} else {
		bool maximized = false;

		// Store state if any subwindow exist.
		if (ui->mdiArea->currentSubWindow() != nullptr) {
			maximized = ui->mdiArea->currentSubWindow()->isMaximized();
		}

		// Switching to QMdiArea::SubWindowView mode causes
		// the windows to change from maximized to normal.
		ui->mdiArea->setViewMode(QMdiArea::SubWindowView);

		// Therefore, if necessary, we restore the maximized window.
		if (maximized) {
			for (auto window : ui->mdiArea->subWindowList()) {
				window->showMaximized();
			}
		}
	}
}

void GUI::WorkShop::MainWindow::createDiffApp()
{
	if (m_diffApp == nullptr) {
		m_diffApp = new KDiff3App(ui->splitter, "DiffApp", app()->fileFilter());
		connect(m_diffApp, &KDiff3App::lineClicked, app(), &GCodeWorkShop::goToLine);
	}
}

void GUI::WorkShop::MainWindow::doDiffL()
{
	QString fileName;

	createDiffApp();

	if (m_diffApp != nullptr) {
		m_toolActions->diffLeft()->setEnabled(false);
		m_toolActions->diffRight()->setEnabled(false);
		m_toolActions->diff()->setEnabled(false);

		m_toolActions->diff()->setChecked(true);

		if (app()->activeDocument()) {
			fileName = app()->activeDocument()->filePath();
		}

		if (fileName.isEmpty()) {
			fileName = app()->lastOpenedPath();
		}

		//        diffApp->close();
		m_diffApp->completeInit(fileName, "");

		m_toolActions->diffLeft()->setEnabled(true);
		m_toolActions->diffRight()->setEnabled(true);
		m_toolActions->diff()->setEnabled(true);
	}
}

void GUI::WorkShop::MainWindow::doDiffR()
{
	QString fileName;

	createDiffApp();

	if (m_diffApp != nullptr) {
		m_toolActions->diffLeft()->setEnabled(false);
		m_toolActions->diffRight()->setEnabled(false);
		m_toolActions->diff()->setEnabled(false);

		m_toolActions->diff()->setChecked(true);

		if (app()->activeDocument()) {
			fileName = app()->activeDocument()->filePath();
		}

		if (fileName.isEmpty()) {
			fileName = app()->lastOpenedPath();
		}

		//        diffApp->close();
		m_diffApp->completeInit("", fileName);

		m_toolActions->diffLeft()->setEnabled(true);
		m_toolActions->diffRight()->setEnabled(true);
		m_toolActions->diff()->setEnabled(true);
	}
}

void GUI::WorkShop::MainWindow::diffTwoFiles(const QString& filename1, const QString& filename2)
{
	createDiffApp();

	if (m_diffApp != nullptr) {
		m_toolActions->diff()->setChecked(true);
		//        diffApp->close();
		m_diffApp->completeInit(filename1, filename2);

		QList<int> sizes;
		sizes.clear();
		sizes.append(0);
		sizes.append(ui->splitter->height());
		ui->splitter->setSizes(sizes);
	}
}

void GUI::WorkShop::MainWindow::diffEditorFile()
{
	Document* doc = app()->activeDocument();

	if (!doc) {
		return;
	}

	createDiffApp();

	if (m_diffApp != nullptr) {
		QString fileName = doc->filePath();

		if (fileName.isEmpty()) {
			return;
		}

		QString fileName1 = QDir::tempPath() + QDir::separator() + QFileInfo(
		                        fileName).baseName() + ".tmp";
		//qDebug() << fileName << fileName1;

		QFile file(fileName1);

		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::warning(this, tr("GCodeWorkShop"),
			                     tr("Cannot write tmp file \"%1\".\n %2")
			                     .arg(QDir::toNativeSeparators(fileName1))
			                     .arg(file.errorString()));
			return;
		}

		file.write(doc->rawData());
		file.close();

		m_toolActions->diff()->setChecked(true);
		//        diffApp->close();
		m_diffApp->completeInit(fileName, fileName1);

		if (file.exists()) {
			file.remove();
		}

		//      QList<int> sizes;
		//      sizes.clear();
		//      sizes.append(0);
		//      sizes.append(splitter->height());
		//      splitter->setSizes(sizes);
	}
}

void GUI::WorkShop::MainWindow::doDiff()
{
	QString fileName;

	if (m_diffApp == nullptr) {
		createDiffApp();

		if (app()->activeDocument()) {
			fileName = app()->activeDocument()->filePath();
		}

		if (fileName.isEmpty()) {
			fileName = app()->lastOpenedPath();
		}

		//        diffApp->close();
		m_diffApp->completeInit(QFileInfo(fileName).canonicalPath(), QFileInfo(fileName).canonicalPath());

	} else if (!m_toolActions->diff()->isChecked()) {
		m_toolActions->diff()->setChecked(false);
		m_diffApp->close();
		m_diffApp = nullptr;
	}
}

void GUI::WorkShop::MainWindow::createSerialToolBar()
{
	if (m_serialToolBar == nullptr) {
		m_serialToolBar = new GUI::SerialToolBar(app(), this);
		addToolBar(Qt::TopToolBarArea, m_serialToolBar);
		m_serialToolBar->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(m_serialToolBar, &QToolBar::visibilityChanged, m_toolActions->showSerialToolBar(), &QAction::setChecked);
		connect(m_serialToolBar, &QObject::destroyed, this, [this]() {
			m_serialToolBar->saveSettings(Medium::instance().settings());
			m_serialToolBar = nullptr;
		});
		connect(app(), &GCodeWorkShop::loadSettings, m_serialToolBar, &GUI::SerialToolBar::loadSettings);
		connect(app(), &GCodeWorkShop::saveSettings, m_serialToolBar, &GUI::SerialToolBar::saveSettings);
		m_serialToolBar->loadSettings(Medium::instance().settings());
	} else if (!m_toolActions->showSerialToolBar()->isChecked()) {
		m_serialToolBar->close();
		return;
	} else {
		m_serialToolBar->show();
		m_toolActions->showSerialToolBar()->setChecked(true);
	}

	m_serialToolBar->loadSerialConfignames();
	updateCurrentSerialConfig();
}

void GUI::WorkShop::MainWindow::updateCurrentSerialConfig()
{
	bool hasMdiChild = (app()->activeDocument() != nullptr);

	if (hasMdiChild && (m_serialToolBar != nullptr)) {
		QDir dir = app()->activeDocument()->dir();
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		dir.setSorting(QDir::Name);
		dir.setNameFilters(QStringList("*.ini"));

		QFileInfoList list = dir.entryInfoList();

		if (!list.isEmpty()) {
			QFileInfo name = list.at(0);
			m_serialToolBar->selectProfile(name.baseName());
		}
	}
}

void GUI::WorkShop::MainWindow::showFindReplaceToolBar(bool replace)
{
	bool intCapsLock = false;
	DocumentWidgetProperties* docprop = app()->documentManager()->documentWidgetProperties(GCoder::DOCUMENT_TYPE).get();
	GCoderWidgetProperties* prop = dynamic_cast<GCoderWidgetProperties*>(docprop);

	if (prop) {
		intCapsLock = prop->intCapsLock;
	}

	m_findToolBar->setCapsLockEnable(intCapsLock);
	m_findToolBar->show();
	m_findToolBar->activate(replace);
}

void GUI::WorkShop::MainWindow::setHighLightMode()
{
	bool ok;
	GCoderDocument* gdoc = app()->activeGCoderDocument();
	int id = m_statusBarHighlight->currentData().toInt(&ok);

	if (gdoc && ok) {
		gdoc->setHighlightMode(id);
		gdoc->widget()->setFocus(Qt::MouseFocusReason);
	}
}

void GUI::WorkShop::MainWindow::changeSession(QAction* action)
{
	app()->changeSession(action->text());
}

void GUI::WorkShop::MainWindow::closeEvent(QCloseEvent* event)
{
	if (!app()->close()) {
		event->ignore();
	}
}

void GUI::WorkShop::MainWindow::resizeEvent(QResizeEvent* event)
{
	if (windowState() == Qt::WindowNoState && event->oldSize().isValid()) {
		mMWConfig.size = event->size();
	}

	QMainWindow::resizeEvent(event);
}

void GUI::WorkShop::MainWindow::moveEvent(QMoveEvent* event)
{
	if (windowState() == Qt::WindowNoState) {
		mMWConfig.pos = geometry().topLeft();
	}

	QMainWindow::moveEvent(event);
}

void GUI::WorkShop::MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (!event->mimeData()->hasUrls()) {
		QMainWindow::dragEnterEvent(event);
		return;
	}

	event->acceptProposedAction();
}

void GUI::WorkShop::MainWindow::dropEvent(QDropEvent* event)
{
	if (!event->mimeData()->hasUrls()) {
		QMainWindow::dropEvent(event);
		return;
	}

	for (const QUrl& url : event->mimeData()->urls()) {
		app()->openFile(url.toString(QUrl::PreferLocalFile));
	}

	event->acceptProposedAction();
}

void GUI::WorkShop::MainWindow::createMenus()
{
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_fileMenu->addAction(m_fileActions->makeNew());
	m_fileMenu->addAction(m_fileActions->open());
	m_fileMenu->addAction(m_fileActions->openExample());

	m_fileMenu->addSeparator();
	m_recentFileMenu = m_fileMenu->addMenu(tr("&Recent files"));
	m_recentFileMenu->setIcon(QIcon(":/images/document-open-recent.png"));
	connect(m_recentFileMenu, &QMenu::triggered, app(), [this](QAction * act) {
		app()->openFile(act->data().toString(), "");
	});
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileActions->save());
	m_fileMenu->addAction(m_fileActions->saveAs());
	m_fileMenu->addAction(m_fileActions->saveAll());
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileActions->findFiles());
	m_fileMenu->addSeparator();

	m_sessionsMenu = m_fileMenu->addMenu(tr("Sessions"));
	connect(m_sessionsMenu, &QMenu::triggered, this, &MainWindow::changeSession);
	m_fileMenu->addAction(m_fileActions->sessionManager());

	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileActions->print());
	m_fileMenu->addAction(m_fileActions->printPreview());
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileActions->close());
	m_fileMenu->addAction(m_fileActions->closeAll());
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileActions->exit());

	m_editMenu = menuBar()->addMenu(tr("&Edit"));
	m_editMenu->addAction(m_editActions->undo());
	m_editMenu->addAction(m_editActions->redo());
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_editActions->cut());
	m_editMenu->addAction(m_editActions->copy());
	m_editMenu->addAction(m_editActions->paste());
	m_editMenu->addAction(m_editActions->del());
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_editActions->selectAll());
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_editActions->find());
	m_editMenu->addAction(m_editActions->replace());

	m_editMenu->addSeparator();
	m_editMenu->addAction(m_addonsActions->semiComment());
	m_editMenu->addAction(m_addonsActions->paraComment());
	m_blockSkipMenu = m_editMenu->addMenu(tr("&Block Skip"));
	m_blockSkipMenu->setIcon(QIcon(":/images/blockskip.png"));
	m_blockSkipMenu->addAction(m_addonsActions->blockSkipIncrement());
	m_blockSkipMenu->addAction(m_addonsActions->blockSkipDecrement());
	m_blockSkipMenu->addAction(m_addonsActions->blockSkipRemove());
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_editActions->readOnly());
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_editActions->config());

	m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
	m_toolsMenu->addAction(m_toolActions->showSerialToolBar());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_addonsActions->insertSpaces());
	m_toolsMenu->addAction(m_addonsActions->removeSpaces());
	m_toolsMenu->addAction(m_addonsActions->dot());
	m_toolsMenu->addAction(m_addonsActions->insertEmptyLines());
	m_toolsMenu->addAction(m_addonsActions->removeEmptyLines());
	m_toolsMenu->addAction(m_addonsActions->cleanUp());
	m_toolsMenu->addAction(m_addonsActions->swapAxes());
	m_toolsMenu->addAction(m_addonsActions->splitProgramms());
	m_toolsMenu->addAction(m_addonsActions->renumber());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_toolActions->diff());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_toolActions->diffLeft());
	m_toolsMenu->addAction(m_toolActions->diffRight());
	m_toolsMenu->addAction(m_toolActions->diffEditor());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_addonsActions->bhc());
	m_toolsMenu->addAction(m_addonsActions->feeds());
	m_toolsMenu->addAction(m_addonsActions->triangle());
	m_toolsMenu->addAction(m_addonsActions->chamfer());
	m_toolsMenu->addAction(m_addonsActions->i2m());
	m_toolsMenu->addAction(m_addonsActions->i2mProg());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_addonsActions->compileMacro());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_toolActions->inLineCalc());
	m_toolsMenu->addAction(m_toolActions->calc());

	m_windowMenu = menuBar()->addMenu(tr("&Window"));
	connect(m_windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

	menuBar()->addSeparator();

	m_helpMenu = menuBar()->addMenu(tr("&Help"));
	m_helpMenu->addAction(m_helpActions->createGlobalToolTips());
	m_helpMenu->addAction(m_helpActions->createUserToolTips());
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_helpActions->about());
	m_helpMenu->addAction(m_helpActions->aboutQt());
}

void GUI::WorkShop::MainWindow::createToolBars()
{
	m_fileToolBar = addToolBar(tr("File"));
	m_fileToolBar->setObjectName("File");
	m_fileToolBar->addAction(m_fileActions->makeNew());
	m_fileToolBar->addAction(m_fileActions->open());
	m_fileToolBar->addAction(m_fileActions->save());
	m_fileToolBar->addAction(m_fileActions->saveAll());
	m_fileToolBar->addAction(m_fileActions->saveAs());
	m_fileToolBar->addSeparator();
	m_fileToolBar->addAction(m_fileActions->findFiles());
	m_fileToolBar->addSeparator();
	m_fileToolBar->addAction(m_fileActions->print());

	m_editToolBar = addToolBar(tr("Edit"));
	m_editToolBar->setObjectName("Edit");
	m_editToolBar->addAction(m_editActions->undo());
	m_editToolBar->addAction(m_editActions->redo());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_editActions->cut());
	m_editToolBar->addAction(m_editActions->copy());
	m_editToolBar->addAction(m_editActions->paste());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_editActions->del());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_editActions->selectAll());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_editActions->find());
	m_editToolBar->addAction(m_editActions->replace());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_toolActions->diff());
	m_editToolBar->addSeparator();
	m_editToolBar->addAction(m_toolActions->diffLeft());
	m_editToolBar->addAction(m_toolActions->diffRight());
	m_editToolBar->addAction(m_toolActions->diffEditor());

	m_toolsToolBar = new QToolBar(tr("Tools"));
	addToolBar(Qt::LeftToolBarArea, m_toolsToolBar);
	m_toolsToolBar->setObjectName("Tools");
	m_toolsToolBar->addAction(m_toolActions->showSerialToolBar());
	m_toolsToolBar->addSeparator();
	m_toolsToolBar->addAction(m_addonsActions->insertSpaces());
	m_toolsToolBar->addAction(m_addonsActions->removeSpaces());
	m_toolsToolBar->addAction(m_addonsActions->cleanUp());
	m_toolsToolBar->addAction(m_addonsActions->dot());
	m_toolsToolBar->addAction(m_addonsActions->swapAxes());
	m_toolsToolBar->addAction(m_addonsActions->renumber());
	m_toolsToolBar->addAction(m_addonsActions->splitProgramms());
	m_toolsToolBar->addSeparator();
	m_toolsToolBar->addAction(m_addonsActions->bhc());
	m_toolsToolBar->addAction(m_addonsActions->feeds());
	m_toolsToolBar->addAction(m_addonsActions->triangle());
	m_toolsToolBar->addAction(m_addonsActions->chamfer());
	m_toolsToolBar->addAction(m_addonsActions->i2m());
	m_toolsToolBar->addAction(m_addonsActions->i2mProg());
	m_toolsToolBar->addSeparator();
	m_toolsToolBar->addAction(m_addonsActions->compileMacro());
	m_toolsToolBar->addSeparator();
	m_toolsToolBar->addAction(m_toolActions->calc());
	m_toolsToolBar->addSeparator();

	m_windowToolBar = addToolBar(tr("Window"));
	m_windowToolBar->setObjectName("Window");
	m_windowToolBar->addAction(windowActions()->previous());
	m_windowToolBar->addAction(windowActions()->next());
}

void GUI::WorkShop::MainWindow::setupStatusBar()
{
	auto statusActions = new GUI::WorkShop::StatusBarActions(app());
	m_statusBarLabel->setFrameShadow(QFrame::Sunken);
	m_statusBarLabel->setFrameShape(QFrame::Box);

	m_statusBarHighlight->setToolTip(tr("Highlight style and tooltip mode"));
	m_statusBarHighlight->setEditable(false);
	m_statusBarHighlight->addItem(tr("AUTO"), MODE_AUTO);
	m_statusBarHighlight->addItem(tr("FANUC"), MODE_FANUC);
	m_statusBarHighlight->addItem(tr("HEIDENHAIN DIALOG"), MODE_HEIDENHAIN);
	m_statusBarHighlight->addItem(tr("HEIDENHAIN ISO"), MODE_HEIDENHAIN_ISO);
	m_statusBarHighlight->addItem(tr("OKUMA OSP"), MODE_OKUMA);
	m_statusBarHighlight->addItem(tr("PHILIPS"), MODE_PHILIPS);
	m_statusBarHighlight->addItem(tr("SINUMERIK OLD"), MODE_SINUMERIK);
	m_statusBarHighlight->addItem(tr("SINUMERIK NEW"), MODE_SINUMERIK_840);
	m_statusBarHighlight->addItem(tr("LinuxCNC"), MODE_LINUXCNC);
	m_statusBarHighlight->addItem(tr("TOOLTIPS"), MODE_TOOLTIPS);

	connect(m_statusBarHighlight, &QComboBox::currentTextChanged, this, &MainWindow::setHighLightMode);
	connect(statusActions->attachToDir(), &QAction::triggered, app(), [this]() {
		app()->attachHighlighterToDir(true);
	});
	connect(statusActions->deAttachToDir(), &QAction::triggered, app(), [this]() {
		app()->attachHighlighterToDir(false);
	});

	auto attachHighlightButton = new QToolButton();
	attachHighlightButton->setDefaultAction(statusActions->attachToDir());
	auto deAttachHighlightButton = new QToolButton();
	deAttachHighlightButton->setDefaultAction(statusActions->deAttachToDir());
	auto readOnlyButton = new QToolButton();
	readOnlyButton->setDefaultAction(m_editActions->readOnly());

	statusBar()->addPermanentWidget(m_statusBarLabel);
	statusBar()->addPermanentWidget(m_statusBarHighlight);
	statusBar()->addPermanentWidget(attachHighlightButton);
	statusBar()->addPermanentWidget(deAttachHighlightButton);
	statusBar()->addPermanentWidget(readOnlyButton);
	statusBar()->setSizeGripEnabled(true);

	statusBar()->showMessage(tr("Ready"));
}

void GUI::WorkShop::MainWindow::createFindToolBar()
{
	m_findToolBar = new GUI::FindToolBar(app(), this);
	m_findToolBar->hide();
	m_findToolBar->setObjectName("Find");
	addToolBar(Qt::BottomToolBarArea, m_findToolBar);
}

void GUI::WorkShop::MainWindow::setupToolTabs()
{
	ui->tabWidget->insertTab(0, createProjectPanel(), QIcon(":/images/project_new.png"), "");
	ui->tabWidget->setTabToolTip(0, tr("Manage project"));
	ui->tabWidget->insertTab(1, createFileBrowserPanel(), QIcon(":/images/folder.png"), "");
	ui->tabWidget->setTabToolTip(1, tr("Browse files"));
	ui->tabWidget->insertTab(2, createClipboardPanel(), QIcon(":/images/clipboard.png"), tr("Clipboard"));
	ui->tabWidget->setTabToolTip(2, tr("Clipboard"));
}

GUI::FileBrowserPanel* GUI::WorkShop::MainWindow::createFileBrowserPanel()
{
	GUI::FileBrowserPanel* fileBrowser = new GUI::FileBrowserPanel(this);
	fileBrowser->setNameFilters(app()->fileFilter());
	connect(app(), &GCodeWorkShop::updateTranslations, fileBrowser, &GUI::FileBrowserPanel::loadTranslations);
	connect(app(), &GCodeWorkShop::loadSettings, fileBrowser, &GUI::FileBrowserPanel::loadSettings);
	connect(app(), &GCodeWorkShop::saveSettings, fileBrowser, &GUI::FileBrowserPanel::saveSettings);
	connect(app(), &GCodeWorkShop::currentDirChanged, fileBrowser, &GUI::FileBrowserPanel::currentDirChanged);
	connect(app(), &GCodeWorkShop::fileFilterChanged, fileBrowser, &GUI::FileBrowserPanel::setNameFilters);
	connect(fileBrowser, &GUI::FileBrowserPanel::fileClicked, app(), &GCodeWorkShop::openFile);
	return fileBrowser;
}

GUI::ProjectPanel* GUI::WorkShop::MainWindow::createProjectPanel()
{
	m_projectPanel = new GUI::ProjectPanel(this);
	connect(app(), &GCodeWorkShop::updateTranslations, m_projectPanel, &GUI::ProjectPanel::loadTranslations);
	connect(app(), &GCodeWorkShop::updateIcons, m_projectPanel, &GUI::ProjectPanel::loadIcons);
	connect(app(), &GCodeWorkShop::loadSettings, m_projectPanel, &GUI::ProjectPanel::loadSettings);
	connect(app(), &GCodeWorkShop::saveSettings, m_projectPanel, &GUI::ProjectPanel::saveSettings);
	connect(m_projectPanel, &GUI::ProjectPanel::fileClicked, app(), &GCodeWorkShop::openFile);
	return m_projectPanel;
}

GUI::ClipboardPanel* GUI::WorkShop::MainWindow::createClipboardPanel()
{
	GUI::ClipboardPanel* clipboardPanel = new GUI::ClipboardPanel(this);
	connect(app(), &GCodeWorkShop::updateTranslations, clipboardPanel, &GUI::ClipboardPanel::loadTranslations);
	connect(app(), &GCodeWorkShop::updateIcons, clipboardPanel, &GUI::ClipboardPanel::loadIcons);
	connect(app(), &GCodeWorkShop::loadSettings, clipboardPanel, &GUI::ClipboardPanel::loadSettings);
	connect(app(), &GCodeWorkShop::saveSettings, clipboardPanel, &GUI::ClipboardPanel::saveSettings);
	connect(this, &GUI::WorkShop::MainWindow::clipboardTextChanged, clipboardPanel, &GUI::ClipboardPanel::addText);
	connect(clipboardPanel, &GUI::ClipboardPanel::hitText, this, &GUI::WorkShop::MainWindow::clipboardSetText);
	return clipboardPanel;
}

GUI::FileTablePanel* GUI::WorkShop::MainWindow::createFileTablePanel()
{
	DocumentManager* documentManager = app()->documentManager();
	GUI::FileTablePanel* fileTablePanel = new GUI::FileTablePanel(documentManager);
	connect(app(), &GCodeWorkShop::updateTranslations, fileTablePanel, &GUI::FileTablePanel::loadTranslations);
	connect(app(), &GCodeWorkShop::updateIcons, fileTablePanel, &GUI::FileTablePanel::loadIcons);
	connect(documentManager, &DocumentManager::modificationChanged, fileTablePanel, &GUI::FileTablePanel::updateFileList);
	connect(documentManager, &DocumentManager::briefChanged, fileTablePanel, &GUI::FileTablePanel::updateFileList);
	connect(documentManager, &DocumentManager::documentListChanged, fileTablePanel, &GUI::FileTablePanel::updateFileList);
	connect(documentManager, &DocumentManager::activeDocumentChanged, fileTablePanel,
	        &GUI::FileTablePanel::updateFileList);
	return fileTablePanel;
}
