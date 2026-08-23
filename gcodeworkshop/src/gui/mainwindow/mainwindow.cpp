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
#include <QDialog>                             // for QDialog
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
#include <QPointer>                            // for QPointer
#include <QPushButton>                         // for QPushButton
#include <QRect>                               // for QRect
#include <QResizeEvent>                        // for QResizeEvent
#include <QSettings>                           // for QSettings
#include <QSize>                               // for QSize
#include <QSplitter>                           // for QSplitter
#include <QStatusBar>                          // for QStatusBar
#include <QString>                             // for QString, operator+
#include <QStringList>                         // for QStringList
#include <QTabBar>                             // for QTabBar
#include <QTabWidget>                          // for QTabWidget
#include <QToolBar>                            // for QToolBar
#include <QToolButton>                         // for QToolButton
#include <QUrl>                                // for QUrl, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic
#include <QVariant>                            // for QVariant
#include <QWidget>                             // for QWidget
#include <Qt>                                  // for WindowState, ToolBarArea, CursorShape, WidgetAttribute, FocusR...
#include <QtGlobal>                            // for QTypeInfo<>::isComplex, QTypeInfo<>::isLarge, QTypeInfo<>::isS...

#include <gcodeworkshop.h>
#include <addons-actions.h>                    // for Actions
#include <document.h>                          // for Document
#include <documentmanager.h>                   // for DocumentManager
#include <documentstyle.h>                     // for DocumentStyle
#include <documentwidgetproperties.h>          // for DocumentWidgetProperties
#include <gcoderdocument.h>                    // for GCoderDocument
#include <gcoderstyle.h>                       // for GCoderStyle, HighlightColors
#include <gcoderwidgetproperties.h>            // for GCoderWidgetProperties
#include <kdiff3/kdiff3.h>                     // for KDiff3App
#include <serialportconfigdialog.h>            // for SerialPortConfigDialog
#include <serialporttestdialog.h>              // for SerialPortTestDialog
#include <serialtransmissiondialog.h>          // for SerialTransmissionDialog
#include <utils/keysequencemap.h>              // for KeySequenceMap
#include <utils/medium.h>                      // for Medium

#include "gcoder.h"                            // for DOCUMENT_TYPE
#include "gui/actions/editactions.h"           // for EditActions
#include "gui/actions/fileactions.h"           // for FileActions
#include "gui/actions/helpactions.h"           // for HelpActions
#include "gui/actions/toolactions.h"           // for ToolActions
#include "gui/actions/windowactions.h"         // for WindowActions
#include "gui/clipboard/clipboardpanel.h"      // for ClipboardPanel
#include "gui/defaultkeysequences.h"           // for defaultKeySequence
#include "gui/filebrowser/filebrowserpanel.h"  // for FileBrowserPanel
#include "gui/filetable/filetablepanel.h"      // for FileTablePanel
#include "gui/findinfiles/findinfilespanel.h"  // for FindInFilesPanel
#include "gui/findtoolbar/findtoolbar.h"       // for FindToolBar
#include "gui/mainwindow/mainwindow.h"         // for MainWindow
#include "gui/project/projectpanel.h"          // for ProjectPanel
#include "highlightmode.h"                     // for HighlightMode
#include "sessionmanager.h"

#include "ui_mainwindow.h"

#define EXAMPLES_PATH             "/usr/share/gcodeworkshop/EXAMPLES"


GUI::MainWindow::MainWindow(GCodeWorkShop* application, QWidget* parent)
	: QMainWindow{parent},
	  m_app(application),
	  ui(new Ui::MainWindow())
{
	m_MdiTabbedMode = false;
	m_findInFilesHighlightEnable = true;

	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setAcceptDrops(true);
	app()->documentManager()->setMdiArea(ui->mdiArea);

	serialToolBar = nullptr;
	findFiles = nullptr;

	connect(ui->hideButton, &QPushButton::clicked, this, &MainWindow::hidePanel);

	m_shortcuts.insert(GUI::defaultKeySequence());
	connect(app(), &GCodeWorkShop::loadSettings, [this](QSettings * cfg) {
		m_shortcuts.load(cfg);
		app()->updateShortcuts(m_shortcuts);
	});
	connect(app(), &GCodeWorkShop::saveSettings, [this](QSettings * cfg) {
		m_shortcuts.save(cfg);
	});

	createActions();
	createToolBars();
	createStatusBar();
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

	clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::dataChanged, this, &MainWindow::clipboardChanged);
}

GUI::MainWindow::~MainWindow()
{
	delete ui;
}

GCodeWorkShop* GUI::MainWindow::app()
{
	return m_app;
}

bool GUI::MainWindow::maybeSave()
{
	return m_projectPanel->maybeSave();
}

Addons::Actions* GUI::MainWindow::addonsActions()
{
	return m_addonsActions;
}

GUI::Actions::EditActions* GUI::MainWindow::editActions()
{
	return m_editActions;
}

GUI::Actions::FileActions* GUI::MainWindow::fileActions()
{
	return m_fileActions;
}

GUI::Actions::HelpActions* GUI::MainWindow::helpActions()
{
	return m_helpActions;
}

GUI::Actions::ToolActions* GUI::MainWindow::toolActions()
{
	return m_toolActions;
}

GUI::Actions::WindowActions* GUI::MainWindow::windowActions()
{
	return m_windowActions;
}

QComboBox* GUI::MainWindow::highlightComboBox()
{
	return highlightTypeCombo;
}

void GUI::MainWindow::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::MainWindow::loadIcons()
{

}

void GUI::MainWindow::loadSettings(QSettings* cfg)
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
		showFindToolBar();
	}

	restoreState(state);

	setMdiTabbedMode(cfg->value("TabbetMode", false).toBool());

	ui->vSplitter->restoreState(cfg->value("VSplitterState", QByteArray()).toByteArray());

	panelState = cfg->value("ProjectPanelState", QByteArray()).toByteArray();
	ui->hSplitter->restoreState(panelState);
	panelHidden = cfg->value("PanelHidden", false).toBool();

	if (panelHidden) {
		ui->vSplitter->hide();
		ui->frame->setMaximumWidth(ui->hideButton->width());
		ui->hideButton->setText(">>");
	}

	ui->tabWidget->setCurrentIndex(cfg->value("TabCurrentIndex", 0).toInt());
}

void GUI::MainWindow::saveSettings(QSettings* cfg) const
{
	cfg->beginGroup("mainwindow");
	cfg->setValue("pos", mMWConfig.pos);
	cfg->setValue("size", mMWConfig.size);
	cfg->setValue("maximized", isMaximized());
	cfg->setValue("state", saveState());
	cfg->endGroup();

	cfg->setValue("TabbetMode", m_MdiTabbedMode);

	cfg->setValue("SerialToolbarShown", (!serialToolBar.isNull()));

	if (!serialToolBar.isNull()) {
		cfg->beginGroup("SerialPortConfigs");
		cfg->setValue("CurrentSerialPortSettings", configBox->currentText());
		cfg->endGroup();
	}

	cfg->setValue("VSplitterState", ui->vSplitter->saveState());
	cfg->setValue("TabCurrentIndex", ui->tabWidget->currentIndex());

	if (panelHidden) {
		cfg->setValue("ProjectPanelState", panelState);
	} else {
		cfg->setValue("ProjectPanelState", ui->hSplitter->saveState());
	}

	cfg->setValue("PanelHidden", panelHidden);

	cfg->setValue("FindToolBarShown", !m_findToolBar->isHidden());
	m_findToolBar->saveSettings(cfg);
}

void GUI::MainWindow::updateStatusBar()
{
	GCoderDocument* gdoc = app()->activeGCoderDocument();

	if (gdoc) {
		int id = highlightTypeCombo->findData(gdoc->highlightMode());
		highlightTypeCombo->blockSignals(true);
		highlightTypeCombo->setCurrentIndex(id);
		highlightTypeCombo->blockSignals(false);

		int line = gdoc->currentLine();
		int column = gdoc->currentColumn();

		labelStat1->setText(tr(" Col: ") + QString::number(column + 1) +
		                    tr("  Line: ") + QString::number(line) +
		                    (gdoc->isModified() ? tr("  <b>Modified</b>  ") : " ") +
		                    (gdoc->isReadOnly() ? tr(" Read only  ") : " ") +
		                    (gdoc->overwriteMode() ? tr(" Overwrite  ") : tr(" Insert ")));

	}
}

void GUI::MainWindow::updateMenus()
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

	m_windowActions->tileHoriz()->setEnabled(hasMdiChild);
	m_windowActions->tileVert()->setEnabled(hasMdiChild);
	m_windowActions->cascade()->setEnabled(hasMdiChild);
	m_windowActions->next()->setEnabled(hasMdiChild);
	m_windowActions->previous()->setEnabled(hasMdiChild);
	m_windowActions->separator()->setVisible(hasMdiChild);
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

	m_editActions->paste()->setEnabled((!clipboard->text().isEmpty()) && hasMdiChildNotReadOnly);

	if (doc) {
		m_fileActions->save()->setText(tr("&Save \"%1\"").arg(doc->fileName()));
		m_fileActions->saveAs()->setText(tr("Save \"%1\" &As...").arg(doc->fileName()));
		m_fileActions->close()->setText(tr("Cl&ose \"%1\"").arg(doc->fileName()));
	}

	updateStatusBar();
}

void GUI::MainWindow::updateWindowMenu()
{
	QString text;

	windowMenu->clear();
	windowMenu->addAction(m_fileActions->close());
	windowMenu->addAction(m_fileActions->closeAll());
	windowMenu->addSeparator();
	windowMenu->addAction(m_windowActions->tileHoriz());
	windowMenu->addAction(m_windowActions->tileVert());
	windowMenu->addAction(m_windowActions->cascade());
	windowMenu->addSeparator();
	windowMenu->addAction(m_windowActions->next());
	windowMenu->addAction(m_windowActions->previous());
	windowMenu->addAction(m_windowActions->separator());

	windowMenu->setAttribute(Qt::WA_AlwaysShowToolTips, true);

	QList<Document*> docList = app()->documentManager()->documentList();
	m_windowActions->separator()->setVisible(!docList.isEmpty());

	for (int i = 0; i < docList.size(); ++i) {
		Document* doc = docList.at(i);

		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1).arg(doc->filePath());
		} else {
			text = tr("%1 %2").arg(i + 1).arg(doc->filePath());
		}

		QAction* action = windowMenu->addAction(text);
		action->setCheckable(true);
		action->setChecked(doc == app()->activeDocument());
		action->setToolTip(doc->brief());
		connect(action, &QAction::triggered, [this, doc]() {
			app()->setActiveDocument(doc);
		});
	}
}


void GUI::MainWindow::updateRecentFilesMenu(const QStringList& fileList)
{
	recentFileMenu->clear();

	for (const QString& file : fileList) {
		QAction* newAc = recentFileMenu->addAction(QIcon(":/images/document-open-recent.png"), file);
		newAc->setData(file);
	}
}

void GUI::MainWindow::updateSessionMenus(const QStringList& sessionList)
{
	sessionsMenu->clear();

	// TODO memory leak?
	QActionGroup* actionGroup = new QActionGroup(sessionsMenu);
	actionGroup->setExclusive(true);

	bool checked = true;

	for (const QString& name : sessionList) {
		QAction* action = actionGroup->addAction(name);
		action->setCheckable(true);
		action->setChecked(checked);
		checked = false;
	}

	sessionsMenu->addActions(actionGroup->actions());
}

void GUI::MainWindow::hidePanel()
{
	ui->hSplitter->setUpdatesEnabled(false);

	if (!panelHidden) {
		panelState = ui->hSplitter->saveState();
		ui->frame->setMaximumWidth(ui->hideButton->width());
		ui->vSplitter->hide();
		ui->hideButton->setText(">>");
		ui->hideButton->setToolTip(tr("Show"));
		panelHidden = true;
	} else {
		panelHidden = false;
		// FIXME
		//app()->fireCurrentDirChanged();
		ui->frame->setMaximumWidth(16777215);
		ui->vSplitter->show();
		ui->hideButton->setText("<<");
		ui->hideButton->setToolTip(tr("Hide"));
		ui->hSplitter->restoreState(panelState);
	}

	ui->hSplitter->updateGeometry();
	ui->hSplitter->setUpdatesEnabled(true);
}

bool GUI::MainWindow::isPanelHidden()
{
	return panelHidden;
}

void GUI::MainWindow::findInFiles()
{
	if (findFiles == nullptr) {
		findFiles = new GUI::FindInFilesPanel(ui->splitter);
		connect(app(), &GCodeWorkShop::intCapsLockChanged, findFiles, &GUI::FindInFilesPanel::setCapsLockEnable);
		auto prop = dynamic_cast<GCoderWidgetProperties*>(app()->documentManager()->documentWidgetProperties(
		                GCoder::DOCUMENT_TYPE).get());

		if (prop) {
			findFiles->setCapsLockEnable(prop->intCapsLock);
		}

		findFiles->loadSettings(Medium::instance().settings());
		connect(findFiles, &FindInFilesPanel::fileClicked, app(), &GCodeWorkShop::openFile);
		connect(app(), &GCodeWorkShop::loadSettings, findFiles, &GUI::FindInFilesPanel::loadSettings);
		connect(app(), &GCodeWorkShop::saveSettings, findFiles, &GUI::FindInFilesPanel::saveSettings);

		if (m_findInFilesHighlightEnable) {
			GCoderStyle* style = dynamic_cast<GCoderStyle*>(app()->documentManager()->documentStyle(GCoder::DOCUMENT_TYPE).get());

			if (style) {
				findFiles->setHighlightColors(style->hColors);
			}
		}

		if (app()->activeDocument()) {
			findFiles->setDir(app()->activeDocument()->path());
		}
	} else if (!m_fileActions->findFiles()->isChecked()) {
		findFiles->saveSettings(Medium::instance().settings());
		findFiles->close();
		findFiles = nullptr;
	} else {
		findFiles->show();
		m_fileActions->findFiles()->setChecked(true);
	}
}

void GUI::MainWindow::clipboardChanged()
{
	if (!isActiveWindow()) {
		return;
	}

	updateMenus();
	emit clipboardTextChanged(clipboard->text());
}

void GUI::MainWindow::clipboardSetText(const QString& text)
{
	if (!text.isEmpty()) {
		clipboard->setText(text);
	}
}

bool GUI::MainWindow::MdiTabbedMode()
{
	return m_MdiTabbedMode;
}

void GUI::MainWindow::setMdiTabbedMode(bool tabbed)
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
		bool maximized = ui->mdiArea->subWindowList().value(0)->isMaximized();
		ui->mdiArea->setViewMode(QMdiArea::SubWindowView);

		if (maximized) {
			foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
				window->showMaximized();
			}
		}
	}
}

void GUI::MainWindow::closeCurrentWindow()
{
	ui->mdiArea->closeActiveSubWindow();
}

void GUI::MainWindow::closeAllMdiWindows()
{
	ui->mdiArea->closeAllSubWindows();
}

void GUI::MainWindow::tileSubWindowsHorizontally()
{
	ui->mdiArea->tileSubWindows();
}

void GUI::MainWindow::tileSubWindowsVertycally()
{
	if (ui->mdiArea->subWindowList().isEmpty()) {
		return;
	}

	QPoint position(0, 0);

	foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
		QRect rect(0, 0, ui->mdiArea->width(),
		           ui->mdiArea->height() / ui->mdiArea->subWindowList().count());
		window->setGeometry(rect);
		window->move(position);
		position.setY(position.y() + window->height());
	}
}

void GUI::MainWindow::cascadeSubWindows()
{
	ui->mdiArea->cascadeSubWindows();
}

void GUI::MainWindow::activateNextSubWindow()
{
	ui->mdiArea->activateNextSubWindow();
}

void GUI::MainWindow::activatePreviousSubWindow()
{
	ui->mdiArea->activatePreviousSubWindow();
}

void GUI::MainWindow::createDiffApp()
{
	if (diffApp == nullptr) {
		diffApp = new KDiff3App(ui->splitter, "DiffApp", app()->fileFilter());
		// FIXME
		//connect(diffApp, &KDiff3App::lineClicked, app(), &GCodeWorkShop::goToLine);
	}
}

void GUI::MainWindow::doDiffL()
{
	QString fileName;

	createDiffApp();

	if (diffApp != nullptr) {
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
		diffApp->completeInit(fileName, "");

		m_toolActions->diffLeft()->setEnabled(true);
		m_toolActions->diffRight()->setEnabled(true);
		m_toolActions->diff()->setEnabled(true);
	}
}

void GUI::MainWindow::doDiffR()
{
	QString fileName;

	createDiffApp();

	if (diffApp != nullptr) {
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
		diffApp->completeInit("", fileName);

		m_toolActions->diffLeft()->setEnabled(true);
		m_toolActions->diffRight()->setEnabled(true);
		m_toolActions->diff()->setEnabled(true);
	}
}

void GUI::MainWindow::diffTwoFiles(const QString& filename1, const QString& filename2)
{
	createDiffApp();

	if (diffApp != nullptr) {
		m_toolActions->diff()->setChecked(true);
		//        diffApp->close();
		diffApp->completeInit(filename1, filename2);

		QList<int> sizes;
		sizes.clear();
		sizes.append(0);
		sizes.append(ui->splitter->height());
		ui->splitter->setSizes(sizes);
	}
}

void GUI::MainWindow::diffEditorFile()
{
	Document* doc = app()->activeDocument();

	if (!doc) {
		return;
	}

	createDiffApp();

	if (diffApp != nullptr) {
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
		diffApp->completeInit(fileName, fileName1);

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

void GUI::MainWindow::doDiff()
{
	QString fileName;

	if (diffApp == nullptr) {
		createDiffApp();

		if (app()->activeDocument()) {
			fileName = app()->activeDocument()->filePath();
		}

		if (fileName.isEmpty()) {
			fileName = app()->lastOpenedPath();
		}

		//        diffApp->close();
		diffApp->completeInit(QFileInfo(fileName).canonicalPath(), QFileInfo(fileName).canonicalPath());

	} else if (!m_toolActions->diff()->isChecked()) {
		m_toolActions->diff()->setChecked(false);
		diffApp->close();
		diffApp = nullptr;
	}
}

void GUI::MainWindow::createSerialToolBar()
{
	if (serialToolBar == nullptr) {
		serialToolBar = new QToolBar(tr("Serial port toolbar"));
		addToolBar(Qt::TopToolBarArea, serialToolBar);
		serialToolBar->setObjectName("SerialToolBar");

		configPortAct = new QAction(QIcon(":/images/serialconfig.png"), tr("Serial port configuration"),
		                            this);
		//configPortAct->setShortcut(tr("F3"));
		configPortAct->setToolTip(tr("Serial port configuration"));
		connect(configPortAct, SIGNAL(triggered()), this, SLOT(serialConfig()));

		receiveAct = new QAction(QIcon(":/images/receive.png"), tr("Receive new file"), this);
		//receiveAct->setShortcut(tr("Shift+F3"));
		receiveAct->setToolTip(tr("Receive new file"));
		connect(receiveAct, SIGNAL(triggered()), this, SLOT(receiveButtonClicked()));

		sendAct = new QAction(QIcon(":/images/send.png"), tr("Send current file"), this);
		//sendAct->setShortcut(tr("F3"));
		sendAct->setToolTip(tr("Send current file"));
		connect(sendAct, SIGNAL(triggered()), this, SLOT(sendButtonClicked()));

		attachToDirAct = new QAction(QIcon(":/images/attach.png"),
		                             tr("Attach current port settings to current directory of programs"), this);
		//attachToDirAct->setShortcut(tr("F3"));
		attachToDirAct->setToolTip(tr("Attach current port settings to current directory of programs"));
		connect(attachToDirAct, SIGNAL(triggered()), this, SLOT(attachToDirButtonClicked()));

		deAttachToDirAct = new QAction(QIcon(":/images/deattach.png"),
		                               tr("Remove settings from the current directory"), this);
		//deAttachToDirAct->setShortcut(tr("F3"));
		deAttachToDirAct->setToolTip(tr("Remove settings from the directory"));
		connect(deAttachToDirAct, SIGNAL(triggered()), this, SLOT(deAttachToDirButtonClicked()));

		diagAct = new QAction(QIcon(":/images/serialtest.png"), tr("Check serial port settings"), this);
		//diagAct->setShortcut(tr("F3"));
		diagAct->setToolTip(tr("Check serial port settings"));
		connect(diagAct, SIGNAL(triggered()), this, SLOT(serialConfigTest()));

		serialCloseAct = new QAction(QIcon(":/images/close_small.png"),
		                             tr("Close send/receive toolbar"), this);
		serialCloseAct->setToolTip(tr("Close send/receive toolbar"));
		connect(serialCloseAct, SIGNAL(triggered()), this, SLOT(closeSerialToolbar()));

		commAppAct = new QAction(QIcon(":/images/spserver.png"),
		                         tr("Start application \"Serial port file server\""), this);
		//diagAct->setShortcut(tr("F3"));
		commAppAct->setToolTip(tr("Start application \"Serial port file server\""));
		connect(commAppAct, SIGNAL(triggered()), this, SLOT(startSerialPortServer()));

		configBox = new QComboBox();
		configBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		configBox->setDuplicatesEnabled(false);

		//serialToolBar->addSeparator();
		serialToolBar->addAction(commAppAct);
		serialToolBar->addAction(attachToDirAct);
		serialToolBar->addAction(deAttachToDirAct);
		serialToolBar->addSeparator();
		serialToolBar->addAction(diagAct);
		serialToolBar->addWidget(configBox);
		serialToolBar->addAction(configPortAct);
		serialToolBar->addSeparator();
		serialToolBar->addAction(receiveAct);
		serialToolBar->addSeparator();
		serialToolBar->addAction(sendAct);

		serialToolBar->addSeparator();
		serialToolBar->addAction(serialCloseAct);
	} else if (!m_toolActions->showSerialToolBar()->isChecked()) {
		closeSerialToolbar();
		return;
	} else {
		serialToolBar->show();
		m_toolActions->showSerialToolBar()->setChecked(true);
	}

	loadSerialConfignames();
	configBox->adjustSize();
	updateCurrentSerialConfig();
}

void GUI::MainWindow::updateCurrentSerialConfig()
{
	bool hasMdiChild = (app()->activeDocument() != nullptr);

	if (hasMdiChild && (serialToolBar != nullptr)) {
		QDir dir = app()->activeDocument()->dir();
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		dir.setSorting(QDir::Name);
		dir.setNameFilters(QStringList("*.ini"));

		QFileInfoList list = dir.entryInfoList();

		if (!list.isEmpty()) {
			QFileInfo name = list.at(0);
			int id = configBox->findText(name.baseName());

			if (id >= 0) {
				configBox->setCurrentIndex(id);
			}
		}
	}
}

void GUI::MainWindow::loadSerialConfignames()
{
	int id;
	QStringList list;
	QString item;

	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("SerialPortConfigs");

	configBox->clear();
	list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
	list.sort();
	configBox->addItems(list);
	item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
	id = configBox->findText(item);

	if (id >= 0) {
		configBox->setCurrentIndex(id);
	}

	settings.endGroup();
}

void GUI::MainWindow::serialConfig()
{
	SerialPortConfigDialog* serialConfigDialog = new SerialPortConfigDialog(this, configBox->currentText());

	if (serialConfigDialog->exec() == QDialog::Accepted) {
		loadSerialConfignames();
	}
}

void GUI::MainWindow::serialConfigTest()
{
	SerialPortTestDialog* trDialog = new SerialPortTestDialog(this);

	trDialog->show();
}

void GUI::MainWindow::sendButtonClicked()
{
	QString tx;
	GCoderDocument* gdoc = app()->activeGCoderDocument();

	if (!gdoc) {
		return;
	}

	receiveAct->setEnabled(false);
	sendAct->setEnabled(false);
	commAppAct->setEnabled(false);
	QApplication::setOverrideCursor(Qt::BusyCursor);

	tx.append(gdoc->text());

	SerialTransmissionDialog transmissionDialog(this);
	transmissionDialog.sendData(tx, configBox->currentText());

	receiveAct->setEnabled(true);
	sendAct->setEnabled(true);
	commAppAct->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

void GUI::MainWindow::receiveButtonClicked()
{
	receiveAct->setEnabled(false);
	sendAct->setEnabled(false);
	commAppAct->setEnabled(false);
	QApplication::setOverrideCursor(Qt::BusyCursor);

	SerialTransmissionDialog transmissionDialog(this);
	QStringList progList = transmissionDialog.receiveData(configBox->currentText());

	if (!progList.isEmpty()) {
		int id = configBox->currentIndex();

		QStringList::const_iterator it = progList.constBegin();

		if ((*it) == "#FILE_LIST#") {
			it++;

			while (it != progList.constEnd()) {
				app()->openFile(*it);
				it++;
			}
		} else {
			if (!(*it).isEmpty() && !(*it).isNull()) {
				GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(app()->newFile());

				if (gdoc) {
					gdoc->clear();
					gdoc->insertText(*it);
					gdoc->setHighlightMode(MODE_AUTO);
					//activeWindow->setReadOnly(defaultMdiWindowProperites.defaultReadOnly);
					gdoc->clearUndoRedoStacks();
				}
			}
		}

		configBox->setCurrentIndex(id);
	}

	receiveAct->setEnabled(true);
	sendAct->setEnabled(true);
	commAppAct->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

void GUI::MainWindow::attachToDirButtonClicked(bool attach)
{
	QFile file;

	bool hasMdiChild = (app()->activeDocument() != 0);

	if (hasMdiChild && (serialToolBar != nullptr)) {
		QDir dir = app()->activeDocument()->dir();
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		dir.setSorting(QDir::Name);
		dir.setNameFilters(QStringList("*.ini"));

		QFileInfoList list = dir.entryInfoList();

		for (const QFileInfo& fileInfo : list) {
			file.setFileName(fileInfo.absoluteFilePath());
			file.remove();
		}

		if (attach) {
			file.setFileName(app()->activeDocument()->path() + "/" + configBox->currentText() + ".ini");
			file.open(QIODevice::ReadWrite);
			file.close();;
		}
	}
}

void GUI::MainWindow::deAttachToDirButtonClicked()
{
	attachToDirButtonClicked(false);
}

void GUI::MainWindow::closeSerialToolbar()
{
	serialToolBar->close();
	delete (serialToolBar);
	serialToolBar = nullptr;
	m_toolActions->showSerialToolBar()->setChecked(false);
}

void GUI::MainWindow::showFindReplaceToolBar(bool replace)
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

void GUI::MainWindow::showFindToolBar()
{
	showFindReplaceToolBar(false);
}

void GUI::MainWindow::showReplaceToolBar()
{
	showFindReplaceToolBar(true);
}

void GUI::MainWindow::setHighLightMode(int mode)
{
	bool ok;
	GCoderDocument* gdoc = app()->activeGCoderDocument();
	int id = highlightTypeCombo->itemData(mode).toInt(&ok);

	if (gdoc && ok) {
		gdoc->setHighlightMode(id);
		gdoc->widget()->setFocus(Qt::MouseFocusReason);
	}
}

void GUI::MainWindow::changeSession(QAction* action)
{
	app()->changeSession(action->text());
}

void GUI::MainWindow::closeEvent(QCloseEvent* event)
{
	if (!app()->close()) {
		event->ignore();
	}
}

void GUI::MainWindow::resizeEvent(QResizeEvent* event)
{
	if (windowState() == Qt::WindowNoState && event->oldSize().isValid()) {
		mMWConfig.size = event->size();
	}

	QMainWindow::resizeEvent(event);
}

void GUI::MainWindow::moveEvent(QMoveEvent* event)
{
	if (windowState() == Qt::WindowNoState) {
		mMWConfig.pos = geometry().topLeft();
	}

	QMainWindow::moveEvent(event);
}

void GUI::MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (!event->mimeData()->hasUrls()) {
		QMainWindow::dragEnterEvent(event);
		return;
	}

	event->acceptProposedAction();
}

void GUI::MainWindow::dropEvent(QDropEvent* event)
{
	if (!event->mimeData()->hasUrls()) {
		QMainWindow::dropEvent(event);
		return;
	}

	for (QUrl url : event->mimeData()->urls()) {
		// Create signal
		app()->openFile(url.toString(QUrl::PreferLocalFile));
	}

	event->acceptProposedAction();
}

void GUI::MainWindow::createActions()
{
	m_addonsActions = new Addons::Actions(app());
	m_editActions = new GUI::Actions::EditActions(this);
	m_fileActions = new GUI::Actions::FileActions(this);
	m_fileActions->openExample()->setEnabled(QDir(EXAMPLES_PATH).exists()
	        || QDir(QApplication::applicationDirPath() + "../" + "examples").exists()
	        || QDir(QApplication::applicationDirPath() + "../../" + "examples").exists());
	m_helpActions = new GUI::Actions::HelpActions(this);
	m_toolActions = new GUI::Actions::ToolActions(this);
	m_windowActions = new GUI::Actions::WindowActions(this);
}

void GUI::MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(m_fileActions->makeNew());
	fileMenu->addAction(m_fileActions->open());
	fileMenu->addAction(m_fileActions->openExample());

	fileMenu->addSeparator();
	recentFileMenu = fileMenu->addMenu(tr("&Recent files"));
	recentFileMenu->setIcon(QIcon(":/images/document-open-recent.png"));
	connect(recentFileMenu, SIGNAL(triggered(QAction*)), this, SLOT(fileOpenRecent(QAction*)));
	fileMenu->addSeparator();
	fileMenu->addAction(m_fileActions->save());
	fileMenu->addAction(m_fileActions->saveAs());
	fileMenu->addAction(m_fileActions->saveAll());
	fileMenu->addSeparator();
	fileMenu->addAction(m_fileActions->findFiles());
	fileMenu->addSeparator();

	sessionsMenu = fileMenu->addMenu(tr("Sessions"));
	connect(sessionsMenu, &QMenu::triggered, this, &MainWindow::changeSession);
	fileMenu->addAction(m_fileActions->sessionManager());

	fileMenu->addSeparator();
	fileMenu->addAction(m_fileActions->print());
	fileMenu->addAction(m_fileActions->printPreview());
	fileMenu->addSeparator();
	fileMenu->addAction(m_fileActions->close());
	fileMenu->addAction(m_fileActions->closeAll());
	fileMenu->addSeparator();
	fileMenu->addAction(m_fileActions->exit());

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(m_editActions->undo());
	editMenu->addAction(m_editActions->redo());
	editMenu->addSeparator();
	editMenu->addAction(m_editActions->cut());
	editMenu->addAction(m_editActions->copy());
	editMenu->addAction(m_editActions->paste());
	editMenu->addAction(m_editActions->del());
	editMenu->addSeparator();
	editMenu->addAction(m_editActions->selectAll());
	editMenu->addSeparator();
	editMenu->addAction(m_editActions->find());
	editMenu->addAction(m_editActions->replace());

	editMenu->addSeparator();
	editMenu->addAction(m_addonsActions->semiComment());
	editMenu->addAction(m_addonsActions->paraComment());
	blockSkipMenu = editMenu->addMenu(tr("&Block Skip"));
	blockSkipMenu->setIcon(QIcon(":/images/blockskip.png"));
	blockSkipMenu->addAction(m_addonsActions->blockSkipIncrement());
	blockSkipMenu->addAction(m_addonsActions->blockSkipDecrement());
	blockSkipMenu->addAction(m_addonsActions->blockSkipRemove());
	editMenu->addSeparator();
	editMenu->addAction(m_editActions->readOnly());
	editMenu->addSeparator();
	editMenu->addAction(m_editActions->config());

	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(m_toolActions->showSerialToolBar());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_addonsActions->insertSpaces());
	toolsMenu->addAction(m_addonsActions->removeSpaces());
	toolsMenu->addAction(m_addonsActions->dot());
	toolsMenu->addAction(m_addonsActions->insertEmptyLines());
	toolsMenu->addAction(m_addonsActions->removeEmptyLines());
	toolsMenu->addAction(m_addonsActions->cleanUp());
	toolsMenu->addAction(m_addonsActions->swapAxes());
	toolsMenu->addAction(m_addonsActions->splitProgramms());
	toolsMenu->addAction(m_addonsActions->renumber());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_toolActions->diff());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_toolActions->diffLeft());
	toolsMenu->addAction(m_toolActions->diffRight());
	toolsMenu->addAction(m_toolActions->diffEditor());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_addonsActions->bhc());
	toolsMenu->addAction(m_addonsActions->feeds());
	toolsMenu->addAction(m_addonsActions->triangle());
	toolsMenu->addAction(m_addonsActions->chamfer());
	toolsMenu->addAction(m_addonsActions->i2m());
	toolsMenu->addAction(m_addonsActions->i2mProg());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_addonsActions->compileMacro());
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_toolActions->inLineCalc());
	toolsMenu->addAction(m_toolActions->calc());

	windowMenu = menuBar()->addMenu(tr("&Window"));
	connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(m_helpActions->createGlobalToolTips());
	helpMenu->addAction(m_helpActions->createUserToolTips());
	helpMenu->addSeparator();
	helpMenu->addAction(m_helpActions->about());
	helpMenu->addAction(m_helpActions->aboutQt());
}

void GUI::MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setObjectName("File");
	fileToolBar->addAction(m_fileActions->makeNew());
	fileToolBar->addAction(m_fileActions->open());
	fileToolBar->addAction(m_fileActions->save());
	fileToolBar->addAction(m_fileActions->saveAll());
	fileToolBar->addAction(m_fileActions->saveAs());
	fileToolBar->addSeparator();
	fileToolBar->addAction(m_fileActions->findFiles());
	fileToolBar->addSeparator();
	fileToolBar->addAction(m_fileActions->print());

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->setObjectName("Edit");
	editToolBar->addAction(m_editActions->undo());
	editToolBar->addAction(m_editActions->redo());
	editToolBar->addSeparator();
	editToolBar->addAction(m_editActions->cut());
	editToolBar->addAction(m_editActions->copy());
	editToolBar->addAction(m_editActions->paste());
	editToolBar->addSeparator();
	editToolBar->addAction(m_editActions->del());
	editToolBar->addSeparator();
	editToolBar->addAction(m_editActions->selectAll());
	editToolBar->addSeparator();
	editToolBar->addAction(m_editActions->find());
	editToolBar->addAction(m_editActions->replace());
	editToolBar->addSeparator();
	editToolBar->addAction(m_toolActions->diff());
	editToolBar->addSeparator();
	editToolBar->addAction(m_toolActions->diffLeft());
	editToolBar->addAction(m_toolActions->diffRight());
	editToolBar->addAction(m_toolActions->diffEditor());

	toolsToolBar = new QToolBar(tr("Tools"));
	addToolBar(Qt::LeftToolBarArea, toolsToolBar);
	toolsToolBar->setObjectName("Tools");
	toolsToolBar->addAction(m_toolActions->showSerialToolBar());
	toolsToolBar->addSeparator();
	toolsToolBar->addAction(m_addonsActions->insertSpaces());
	toolsToolBar->addAction(m_addonsActions->removeSpaces());
	toolsToolBar->addAction(m_addonsActions->cleanUp());
	toolsToolBar->addAction(m_addonsActions->dot());
	toolsToolBar->addAction(m_addonsActions->swapAxes());
	toolsToolBar->addAction(m_addonsActions->renumber());
	toolsToolBar->addAction(m_addonsActions->splitProgramms());
	toolsToolBar->addSeparator();
	toolsToolBar->addAction(m_addonsActions->bhc());
	toolsToolBar->addAction(m_addonsActions->feeds());
	toolsToolBar->addAction(m_addonsActions->triangle());
	toolsToolBar->addAction(m_addonsActions->chamfer());
	toolsToolBar->addAction(m_addonsActions->i2m());
	toolsToolBar->addAction(m_addonsActions->i2mProg());
	toolsToolBar->addSeparator();
	toolsToolBar->addAction(m_addonsActions->compileMacro());
	toolsToolBar->addSeparator();
	toolsToolBar->addAction(m_toolActions->calc());
	toolsToolBar->addSeparator();

	windowToolBar = addToolBar(tr("Window"));
	windowToolBar->setObjectName("Window");
	windowToolBar->addAction(m_windowActions->previous());
	windowToolBar->addAction(m_windowActions->next());
}

void GUI::MainWindow::createStatusBar()
{
	labelStat1 = new QLabel("    ");

	labelStat1->setFrameShadow(QFrame::Sunken);
	labelStat1->setFrameShape(QFrame::Box);

	highlightTypeCombo = new QComboBox();
	highlightTypeCombo->setToolTip(tr("Highlight style and tooltip mode"));
	highlightTypeCombo->setEditable(false);
	highlightTypeCombo->addItem(tr("AUTO"), MODE_AUTO);
	highlightTypeCombo->addItem(tr("FANUC"), MODE_FANUC);
	highlightTypeCombo->addItem(tr("HEIDENHAIN DIALOG"), MODE_HEIDENHAIN);
	highlightTypeCombo->addItem(tr("HEIDENHAIN ISO"), MODE_HEIDENHAIN_ISO);
	highlightTypeCombo->addItem(tr("OKUMA OSP"), MODE_OKUMA);
	highlightTypeCombo->addItem(tr("PHILIPS"), MODE_PHILIPS);
	highlightTypeCombo->addItem(tr("SINUMERIK OLD"), MODE_SINUMERIK);
	highlightTypeCombo->addItem(tr("SINUMERIK NEW"), MODE_SINUMERIK_840);
	highlightTypeCombo->addItem(tr("LinuxCNC"), MODE_LINUXCNC);
	highlightTypeCombo->addItem(tr("TOOLTIPS"), MODE_TOOLTIPS);

	connect(highlightTypeCombo, SIGNAL(currentIndexChanged(int)), this,
	        SLOT(setHighLightMode(int)));

	attachHighlightToDirAct = new QAction(QIcon(":/images/attach.png"),
	                                      tr("Attach current highlight setting to current directory of programs"), this);
	attachHighlightToDirAct->setToolTip(
	    tr("Attach current highlight setting to current directory of programs"));
	connect(attachHighlightToDirAct, SIGNAL(triggered()), this,
	        SLOT(attachHighlightToDirActClicked()));

	attachHighlightButton = new QToolButton();
	attachHighlightButton->setDefaultAction(attachHighlightToDirAct);

	deAttachHighlightToDirAct = new QAction(QIcon(":/images/deattach.png"),
	                                        tr("Remove highlight settings from the directory"), this);
	deAttachHighlightToDirAct->setToolTip(tr("Remove highlight settings from the directory"));
	connect(deAttachHighlightToDirAct, SIGNAL(triggered()), this,
	        SLOT(deAttachHighlightToDirActClicked()));

	deAttachHighlightButton = new QToolButton();
	deAttachHighlightButton->setDefaultAction(deAttachHighlightToDirAct);

	readOnlyButton = new QToolButton();
	readOnlyButton->setDefaultAction(m_editActions->readOnly());

	//statusBar()->addPermanentWidget(highlightLabel);
	statusBar()->addPermanentWidget(labelStat1);
	statusBar()->addPermanentWidget(highlightTypeCombo);
	statusBar()->addPermanentWidget(attachHighlightButton);
	statusBar()->addPermanentWidget(deAttachHighlightButton);
	statusBar()->addPermanentWidget(readOnlyButton);
	statusBar()->setSizeGripEnabled(true);

	statusBar()->showMessage(tr("Ready"));
}

void GUI::MainWindow::createFindToolBar()
{
	m_findToolBar = new GUI::FindToolBar(app(), this);
	m_findToolBar->hide();
	m_findToolBar->setObjectName("Find");
	addToolBar(Qt::BottomToolBarArea, m_findToolBar);
}

void GUI::MainWindow::setupToolTabs()
{
	ui->tabWidget->insertTab(0, createProjectPanel(), QIcon(":/images/project_new.png"), "");
	ui->tabWidget->setTabToolTip(0, tr("Manage project"));
	ui->tabWidget->insertTab(1, createFileBrowserPanel(), QIcon(":/images/folder.png"), "");
	ui->tabWidget->setTabToolTip(1, tr("Browse files"));
	ui->tabWidget->insertTab(2, createClipboardPanel(), QIcon(":/images/clipboard.png"), tr("Clipboard"));
	ui->tabWidget->setTabToolTip(2, tr("Clipboard"));
}

GUI::FileBrowserPanel* GUI::MainWindow::createFileBrowserPanel()
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

GUI::ProjectPanel* GUI::MainWindow::createProjectPanel()
{
	m_projectPanel = new GUI::ProjectPanel(this);
	connect(app(), &GCodeWorkShop::updateTranslations, m_projectPanel, &GUI::ProjectPanel::loadTranslations);
	connect(app(), &GCodeWorkShop::updateIcons, m_projectPanel, &GUI::ProjectPanel::loadIcons);
	connect(app(), &GCodeWorkShop::loadSettings, m_projectPanel, &GUI::ProjectPanel::loadSettings);
	connect(app(), &GCodeWorkShop::saveSettings, m_projectPanel, &GUI::ProjectPanel::saveSettings);
	connect(m_projectPanel, &GUI::ProjectPanel::fileClicked, app(), &GCodeWorkShop::openFile);
	return m_projectPanel;
}

GUI::ClipboardPanel* GUI::MainWindow::createClipboardPanel()
{
	GUI::ClipboardPanel* clipboardPanel = new GUI::ClipboardPanel(this);
	connect(app(), &GCodeWorkShop::updateTranslations, clipboardPanel, &GUI::ClipboardPanel::loadTranslations);
	connect(app(), &GCodeWorkShop::updateIcons, clipboardPanel, &GUI::ClipboardPanel::loadIcons);
	connect(app(), &GCodeWorkShop::loadSettings, clipboardPanel, &GUI::ClipboardPanel::loadSettings);
	connect(app(), &GCodeWorkShop::saveSettings, clipboardPanel, &GUI::ClipboardPanel::saveSettings);
	connect(this, &GUI::MainWindow::clipboardTextChanged, clipboardPanel, &GUI::ClipboardPanel::addText);
	connect(clipboardPanel, &GUI::ClipboardPanel::hitText, this, &GUI::MainWindow::clipboardSetText);
	return clipboardPanel;
}

GUI::FileTablePanel* GUI::MainWindow::createFileTablePanel()
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
