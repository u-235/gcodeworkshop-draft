/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of GCodeWorkShop.                                   *
 *                                                                         *
 *   GCodeWorkShop is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QAbstractPrintDialog> // for QAbstractPrintDialog
#include <QAction>              // for QAction
#include <QApplication>         // for QApplication
#include <QByteArray>           // for QByteArray
#include <QComboBox>            // for QComboBox
#include <QDesktopServices>     // for QDesktopServices
#include <QDialog>              // for QDialog
#include <QDir>                 // for QDir, operator|
#include <QFile>                // for QFile
#include <QFileDialog>          // for QFileDialog
#include <QFileInfo>            // for QFileInfo, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QFileInfoList
#include <QFileSystemWatcher>   // for QFileSystemWatcher
#include <QIODevice>            // for QIODevice, operator|
#include <QLatin1String>        // for QLatin1String
#include <QList>                // for QList
#include <QMainWindow>          // for QMainWindow
#include <QMenu>                // for QMenu
#include <QMessageBox>          // for QMessageBox, operator|
#include <QObject>              // for SIGNAL, SLOT, emit, QObject
#include <QPageLayout>          // for QPageLayout
#include <QPageSize>            // for QPageSize
#include <QPointer>             // for QPointer
#include <QPrintDialog>         // for QPrintDialog
#include <QPrintPreviewDialog>  // for QPrintPreviewDialog
#include <QPrinter>             // for QPrinter
#include <QProcess>             // for QProcess
#include <QSettings>            // for QSettings
#include <QStatusBar>           // for QStatusBar
#include <QString>              // for QString, operator+, operator!=, operator==
#include <QTextStream>          // for QTextStream
#include <QUrl>                 // for QUrl
#include <QVariant>             // for QVariant
#include <QWidget>              // for QWidget
#include <Qt>                   // for WindowType, CursorShape, ConnectionType, CaseSensitivity, SplitBehavi...
#include <QtGlobal>             // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QT_VERSION, QT_VERSION_C...

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	using QStringList = QList<QString>;
#endif

#include <document.h>                   // for Document
#include <documentinfo.h>               // for DocumentInfo
#include <documentmanager.h>            // for DocumentManager
#include <documentstyle.h>              // for DocumentStyle
#include <documentwidgetproperties.h>   // for DocumentWidgetProperties
#include <gcodefileserver.h>            // for GCodeFileServer
#include <gcoderdocument.h>             // for GCoderDocument
#include <gcoderstyle.h>                // for GCoderStyle
#include <gcoderwidgetproperties.h>     // for GCoderWidgetProperties
#include <gcodeworkshop.h>
#include <kdiff3/common.h>              // for getFilters
#include <utils/gcode-converter.h>      // for Converter
#include <utils/medium.h>               // for Medium
#include <version.h>                    // for GCODEWORKSHOP_VERSION

#include "gcoder.h"                         // for DOCUMENT_TYPE
#include "gcoderinfo.h"                     // for GCoderInfo
#include "gcoderproducer.h"                 // for GCoderProducer
#include "gui/actions/editactions.h"        // for EditActions
#include "gui/actions/toolactions.h"        // for ToolActions
#include "gui/mainwindow/mainwindow.h"      // for MainWindow
#include "gui/newfile/newfiledialog.h"      // for NewFileDialog
#include "gui/session/sessiondialog.h"      // for SessionDialog
#include "gui/setup/setupdialog.h"          // for AppConfig, SetupDialog
#include "highlightmode.h"                  // for HighlightMode
#include "recentfiles.h"                    // for RecentFiles
#include "sessionmanager.h"                 // for SessionManager
#include "tooltips.h"                       // for writeTooltipFile


#define EXAMPLES_PATH             "/usr/share/gcodeworkshop/EXAMPLES"

GCodeWorkShop* GCodeWorkShop::SINGLETON;

GCodeWorkShop* GCodeWorkShop::instance()
{
	if (SINGLETON == 0) {
		SINGLETON = new GCodeWorkShop(&Medium::instance());
	}

	return SINGLETON;
}

GCodeWorkShop::GCodeWorkShop(Medium* medium)
	: QObject(nullptr)
{
	m_mainWindow = nullptr;
	mMedium = medium;
	m_fileServer = nullptr;

	m_MdiWidgetsMaximized = true;
	m_defaultReadOnly = false;
	m_startEmpty = false;
	m_disableFileChangeMonitor = false;
	m_findInFilesHighlightEnable = true;

	m_extensions << "*.nc" <<  "*.cnc";
	m_saveExtension = "*.nc";
	m_saveDirectory = QDir::homePath();

	m_recentFiles = new RecentFiles(this);
	connect(m_recentFiles, SIGNAL(saveRequest()), this, SLOT(recentFilesChanged()));

	m_documentManager = new DocumentManager(this);
	// FIXME
	// m_documentManager->setMdiArea(ui->mdiArea);
	m_documentManager->registerDocumentProducer(new GCoderProducer());
	connect(m_documentManager, SIGNAL(closeRequested(Document*)), this, SLOT(maybeSave(Document*)),
	        Qt::ConnectionType::DirectConnection);
	// FIXME
	//connect(m_documentManager, SIGNAL(redoAvailable(bool)), m_editActions->redo(), SLOT(setEnabled(bool)));
	//connect(m_documentManager, SIGNAL(undoAvailable(bool)), m_editActions->undo(), SLOT(setEnabled(bool)));
	connect(m_documentManager, SIGNAL(customContextMenuRequested(Document*, const QPoint&)), this,
	        SLOT(customContextMenuRequest(Document*, const QPoint&)));
	connect(m_documentManager, SIGNAL(fileWatchRequest(const QString&, bool)), this, SLOT(watchFile(const QString&, bool)));

	m_sessionManager = new SessionManager(m_documentManager, this);
	connect(m_sessionManager, SIGNAL(beforeCurrentSessionChanged()), this, SLOT(beforeCurrentSessionChanged()));
	connect(m_sessionManager, SIGNAL(currentSessionChanged()), this, SLOT(currentSessionChanged()));
	connect(m_sessionManager, SIGNAL(saveRequest()), this, SLOT(sessionsChanged()));

	emit loadSettings(Medium::instance().settings());
	readSettings();
}

GCodeWorkShop::~GCodeWorkShop()
{
	proc = findChild<QProcess*>();

	if (proc) {
		proc->close();
		delete (proc);
	}

	if (m_fileServer) {
		m_fileServer->close();
	}

	if (m_mainWindow) {
		m_mainWindow->deleteLater();
	}
}

QMainWindow* GCodeWorkShop::mainWindow()
{
	if (!m_mainWindow) {
		m_mainWindow = new GUI::MainWindow(this);
		connect(m_recentFiles, &RecentFiles::fileListChanged, m_mainWindow, &GUI::MainWindow::updateRecentFilesMenu);
		connect(m_documentManager, &DocumentManager::activeDocumentChanged, m_mainWindow, &GUI::MainWindow::updateMenus);
		connect(m_documentManager, &DocumentManager::cursorPositionChanged, m_mainWindow, &GUI::MainWindow::updateStatusBar);
		connect(m_documentManager, &DocumentManager::modificationChanged, m_mainWindow, &GUI::MainWindow::updateMenus);
		connect(m_documentManager, &DocumentManager::selectionChanged, m_mainWindow, &GUI::MainWindow::updateMenus);
		connect(m_sessionManager, &SessionManager::sessionListChanged, m_mainWindow, &GUI::MainWindow::updateSessionMenus);
		m_mainWindow->updateMenus();
		m_mainWindow->updateRecentFilesMenu(m_recentFiles->fileList());
		m_mainWindow->updateSessionMenus(m_sessionManager->sessionList());
		m_mainWindow->updateStatusBar();
	}

	return m_mainWindow;
}

DocumentManager* GCodeWorkShop::documentManager() const
{
	return m_documentManager;
}

QStringList GCodeWorkShop::fileFilter() const
{
	return m_extensions;
}

void GCodeWorkShop::setFileFilter(const QStringList& extensions)
{
	m_extensions = extensions;
	emit fileFilterChanged(m_extensions);
}


bool GCodeWorkShop::close()
{
	if (m_fileServer) {
		QMessageBox::StandardButton result = QMessageBox::warning(mainWindow(),
		                                     tr("GCodeWorkShop - Serial port file server"),
		                                     tr("Serial port file server is running.\nClose anyway?"),
		                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if (result == QMessageBox::No) {
			return false;
		}
	}

	if (!m_mainWindow->maybeSave() || !maybeSaveAll()) {
		return false;
	}

	emit saveSettings(Medium::instance().settings());
	writeSettings();
	m_mainWindow->closeAllMdiWindows();
	return true;
}

Document* GCodeWorkShop::newFileFromTemplate()
{
	Document* doc = 0;

	GUI::NewFileDialog* newFileDlg = new GUI::NewFileDialog(mainWindow());
	newFileDlg->setNameFilters(m_extensions);
	newFileDlg->loadSettings(Medium::instance().settings());
	int result = newFileDlg->exec();

	if (result == QDialog::Accepted) {
		newFileDlg->saveSettings(Medium::instance().settings());
		const QString& fileName = newFileDlg->getChosenFile();
		doc = createDocument(GCoder::DOCUMENT_TYPE);

		if (!doc) {
			return nullptr;
		}

		if (!fileName.isEmpty() && !(fileName == tr("EMPTY FILE"))) {
			doc->loadTemplate(fileName);
		} else {
			doc->loadTemplate();
		}

		// TODO replace with DocumentProducer::createDocumentInfo
		DocumentInfo::Ptr info = DocumentInfo::Ptr(new GCoderInfo());
		doc->setDocumentInfo(info);
	}

	newFileDlg->deleteLater();
	return doc;
}

//**************************************************************************************************
//
//**************************************************************************************************

Document* GCodeWorkShop::newFile()
{
	Document* doc = createDocument(GCoder::DOCUMENT_TYPE);

	if (!doc) {
		return nullptr;
	}

	doc->loadTemplate();
	return doc;
}

//**************************************************************************************************
//
//**************************************************************************************************

void GCodeWorkShop::openDir(const QDir& dir)
{
	const QString& filters = getFilters(m_extensions);

	QStringList files = QFileDialog::getOpenFileNames(
	                        mainWindow(),
	                        tr("Select one or more files to open"),
	                        dir.canonicalPath(),
	                        filters, 0);

	for (const QString& fileName : files) {
		openFile(fileName);
	}
}

void GCodeWorkShop::open()
{
	openDir(currentPath());
	statusBarMessage(tr("File loaded"), 5000);
}

void GCodeWorkShop::openExample()
{
	QString dir;

	// TODO: change a path to the files
	if (QDir(EXAMPLES_PATH).exists()) {
		dir = EXAMPLES_PATH;
	} else {
		dir = QApplication::applicationDirPath() + "/" + "EXAMPLES";
	}

	openDir(dir);
	statusBarMessage(tr("File loaded"), 5000);
}

void GCodeWorkShop::openFile(const QString& fileName, const QString& mime)
{
	QFileInfo info(fileName);
	bool gcode = true;

	if (mime.isEmpty()) {
		gcode = m_extensions.contains("*." + info.suffix(), Qt::CaseInsensitive);
	} else {
		gcode = mime.contains("gcode");
	}

	if (gcode) {
		GCoderInfo* info = new GCoderInfo();
		info->filePath = fileName;
		info->readOnly = m_defaultReadOnly;
		info->highlightMode = defaultHighlightMode(QFileInfo(fileName).absolutePath());
		loadFile(DocumentInfo::Ptr(info), true);
	} else {
		QDesktopServices::openUrl(QUrl("file:///" + fileName, QUrl::TolerantMode));
	}
}

bool GCodeWorkShop::saveDocument(Document* doc, bool forceSaveAs)
{
	if (doc->isUntitled() || forceSaveAs) {
		QString oldFileName;

#ifdef Q_OS_LINUX
		QString extText = tr("CNC programs files %1 (%1);;");
#elif defined Q_OS_WIN32
		QString extText = tr("CNC programs files (%1);;");
#elif defined Q_OS_MACX
		QString extText = tr("CNC programs files %1 (%1);;");
#endif

		QString filters = extText.arg(m_saveExtension);

		for (const QString& ext : m_extensions) {
			QString saveExt = extText.arg(ext);

			if (ext != m_saveExtension) {
				filters.append(saveExt);
			}
		}

		filters.append(tr("Text files (*.txt);;" "All files (*.* *)"));

		if (doc->isUntitled()) {
			oldFileName = doc->guessFileName();
		} else {
			oldFileName = doc->fileName();
		}

		if (QFileInfo(oldFileName).suffix() == "") {
			// sometimes when file has no extension QFileDialog::getSaveFileName will not apply chosen filter (extension)
			oldFileName.append(".nc");
		}

		QString newFileName = QFileDialog::getSaveFileName(
		                          mainWindow(),
		                          tr("Save file as..."),
		                          doc->dir().filePath(oldFileName),
		                          filters, nullptr, QFileDialog::DontConfirmOverwrite);

		if (newFileName.isEmpty() || newFileName.isNull()) {
			return false;
		}

		if (QFile::exists(newFileName)) {
			QMessageBox msgBox;
			msgBox.setParent(mainWindow(), Qt::Dialog);
			msgBox.setText(tr("<b>File \"%1\" exists.</b>").arg(newFileName));
			msgBox.setInformativeText(tr("Do you want overwrite it ?"));
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
			msgBox.setDefaultButton(QMessageBox::Discard);
			msgBox.setIcon(QMessageBox::Warning);

			if (msgBox.exec() != QMessageBox::Save) {
				return false;
			}
		}

		doc->setFilePath(newFileName);
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	bool status = doc->save();
	QApplication::restoreOverrideCursor();
	return status;
}

bool GCodeWorkShop::save()
{
	Document* doc = activeDocument();

	if (!doc) {
		return true;
	}

	bool saved = saveDocument(doc, false);

	if (saved) {
		statusBarMessage(tr("File saved"), 5000);
	} else {
		QMessageBox::warning(mainWindow(), tr("GCodeWorkShop"), tr("Cannot write file \"%1\".\n %2")
		                     .arg(doc->filePath()).arg(doc->ioErrorString()));
	}

	return saved;
}

bool GCodeWorkShop::saveAll()
{
	bool saved = true;
	int i = 0;

	for (Document* doc : m_documentManager->documentList()) {
		if (doc->isModified()) {
			if (saveDocument(doc, false)) {
				i++;
			} else {
				saved = false;
				QMessageBox::warning(mainWindow(), tr("GCodeWorkShop"), tr("Cannot write file \"%1\".\n %2")
				                     .arg(doc->filePath()).arg(doc->ioErrorString()));
			}
		}
	}

	statusBarMessage(tr("Saved %1 files").arg(i), 5000);
	return saved;
}

bool GCodeWorkShop::saveAs()
{
	Document* doc = activeDocument();

	if (!doc) {
		return true;
	}

	bool saved = saveDocument(doc, true);

	if (saved) {
		statusBarMessage(tr("File saved"), 5000);
	} else {
		QMessageBox::warning(mainWindow(), tr("GCodeWorkShop"), tr("Cannot write file \"%1\".\n %2")
		                     .arg(doc->filePath()).arg(doc->ioErrorString()));
	}

	return saved;
}

bool GCodeWorkShop::maybeSaveAll()
{
	bool saved = true;

	for (Document* doc : m_documentManager->documentList()) {
		if (!maybeSave(doc)) {
			saved = false;
		}
	}

	return saved;
}

bool GCodeWorkShop::maybeSave(Document* doc)
{
	if (doc->isModified()) {
		QMessageBox msgBox;
		msgBox.setParent(mainWindow(), Qt::Dialog);
		msgBox.setText(tr("<b>File: \"%1\"\n has been modified.</b>").arg(doc->filePath()));
		msgBox.setInformativeText(tr("Do you want to save your changes ?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		msgBox.setIcon(QMessageBox::Warning);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			return saveDocument(doc, false);
			break;

		case QMessageBox::Discard:
			doc->setModified(false);
			return true;
			break;

		case QMessageBox::Cancel:
			return false;
			break;

		default:
			return true;
			break;
		}
	}

	return true;
}

void GCodeWorkShop::printFile()
{
#ifndef QT_NO_PRINTER

	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		QPrinter printer(QPrinter::HighResolution);
		loadPrinterSettings(&printer);

		printer.setOutputFormat(QPrinter::NativeFormat);

		QPrintDialog dialog(&printer, mainWindow());
		dialog.setWindowTitle(tr("Print Document"));

		if (gdoc->hasSelection()) {
			dialog.setOption(QAbstractPrintDialog::PrintSelection);
			printer.setPrintRange(QPrinter::Selection);
		}

		if (dialog.exec() == QDialog::Accepted) {
			printer.setDocName(gdoc->fileName());
			gdoc->print(&printer);
			statusBarMessage(tr("The document was sent to a printer %1...").arg(
			                     printer.printerName()), 5000);
			savePrinterSettings(&printer);
		}
	}

#endif
}

void GCodeWorkShop::filePrintPreview()
{
#ifndef QT_NO_PRINTER

	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		QPrinter printer(QPrinter::HighResolution);
		loadPrinterSettings(&printer);

		printer.setOutputFormat(QPrinter::NativeFormat);

		if (gdoc->hasSelection()) {
			printer.setPrintRange(QPrinter::Selection);
		}

		QPrintPreviewDialog preview(&printer, mainWindow());
		preview.setWindowFlags(Qt::Window);
		connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
		preview.exec();
		savePrinterSettings(&printer);
	}

#endif
}

void GCodeWorkShop::printPreview(QPrinter* printer)
{
#ifndef QT_NO_PRINTER

	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		printer->setDocName(gdoc->fileName());
		gdoc->print(printer);
		statusBarMessage(tr("The document was sent to a printer %1...").arg(
		                     printer->printerName()), 5000);
	}

#endif
}

void GCodeWorkShop::cut()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->cut();
	}
}

void GCodeWorkShop::copy()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->copy();
	}
}

void GCodeWorkShop::selAll()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->selectAll();
	}
}

void GCodeWorkShop::config()
{
	AppConfig config;
	config.editorProperties = *m_documentManager->documentWidgetProperties(GCoder::DOCUMENT_TYPE);
	config.codeStyle = *m_documentManager->documentStyle(GCoder::DOCUMENT_TYPE);
	config.calcBinary = m_calcBinary;
	config.extensions = m_extensions;
	config.saveExtension = m_saveExtension;
	config.saveDirectory = m_saveDirectory;
	config.mdiTabbedMode = m_mainWindow->MdiTabbedMode();
	config.defaultReadOnly = m_defaultReadOnly;
	config.disableFileChangeMonitor = m_disableFileChangeMonitor;
	config.startEmpty = m_startEmpty;
	config.gcodeConverterOptions = GCode::Converter::defaultOptions();
	GUI::SetupDialog* setUpDialog = new GUI::SetupDialog(mainWindow(), &config);

	if (setUpDialog->exec() == QDialog::Accepted) {
		config = setUpDialog->getSettings();
		QSettings* cfg = Medium::instance().settings();
		m_documentManager->setDocumentWidgetProperties(DocumentWidgetProperties::Ptr(new GCoderWidgetProperties(
		            config.editorProperties)));
		config.editorProperties.save(cfg);
		emit intCapsLockChanged(config.editorProperties.intCapsLock);
		m_documentManager->setDocumentStyle(DocumentStyle::Ptr(new GCoderStyle(config.codeStyle)));
		config.codeStyle.save(cfg);
		m_calcBinary = config.calcBinary;
		setFileFilter(config.extensions);
		m_saveExtension = config.saveExtension;
		m_saveDirectory = config.saveDirectory;
		m_mainWindow->setMdiTabbedMode(config.mdiTabbedMode);
		m_defaultReadOnly = config.defaultReadOnly;
		m_disableFileChangeMonitor = config.disableFileChangeMonitor;
		m_startEmpty = config.startEmpty;

		m_documentManager->updateDocuments(GCoder::DOCUMENT_TYPE);

		for (Document* doc : m_documentManager->documentList()) {
			doc->setReadOnly(m_defaultReadOnly);
		}

		GCode::Converter::setDefaultOptions(config.gcodeConverterOptions);
	}

	delete setUpDialog;
}

void GCodeWorkShop::readOnly()
{
	if (activeDocument()) {
		activeDocument()->setReadOnly(m_mainWindow->editActions()->readOnly()->isChecked());
	}

	m_mainWindow->updateMenus();
}

void GCodeWorkShop::goToLine(const QString& fileName, int line)
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		QString childFileName = gdoc->filePath();
		childFileName = QDir().toNativeSeparators(childFileName);

		if (QDir().toNativeSeparators(fileName) != childFileName) {
			return;
		}

		gdoc->goToLine(line);
	}
}

void GCodeWorkShop::diffTwoFiles(const QString& filename1, const QString& filename2)
{
	m_mainWindow->diffTwoFiles(filename1, filename2);
}

void GCodeWorkShop::doCalc()
{
	if (!QFile::exists(m_calcBinary)) {
		QMessageBox::information(mainWindow(), tr("Information"),
		                         tr("Set correct calculator program name in configuration dialog."));
		return;
	}

	proc = findChild<QProcess*>("Calc569");

	if (!proc) {
		proc = new QProcess(this);
		proc->setObjectName("Calc569");
	}

	bool isNotRun;
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
	isNotRun = proc->pid() == 0;
#else
	isNotRun = proc->processId() == 0;
#endif

	if (isNotRun) {
		proc->start(m_calcBinary, QStringList());
	}
}

void GCodeWorkShop::deleteText()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->removeSelectedText();
	}
}

void GCodeWorkShop::paste()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->paste();
	}
}

void GCodeWorkShop::undo()
{
	if (activeDocument()) {
		activeDocument()->undo();
	}
}

void GCodeWorkShop::redo()
{
	if (activeDocument()) {
		activeDocument()->redo();
	}
}

void GCodeWorkShop::activeWindowChanged(QMdiSubWindow* window)
{
	Q_UNUSED(window);
	Document* doc;

	if (m_documentManager->documentList().count() <= 1) {
		m_MdiWidgetsMaximized = true;
	}

	doc = activeDocument();

	if (doc) {
		m_MdiWidgetsMaximized = doc->widget()->parentWidget()->isMaximized();
		statusBarMessage(doc->filePath(), 9000);
	}

	m_mainWindow->updateCurrentSerialConfig();
	fireCurrentDirChanged();
}

void GCodeWorkShop::about()
{
	QMessageBox::about(mainWindow(), tr("About GCodeWorkShop"),
	                   tr("The <b>GCodeWorkShop</b> is text editor for CNC programmers."
	                      "<br>This is a fork of <a href='https://github.com/artur3/EdytorNC'>EdytorNC</a>."
	                      "<P>Version: %1"
	                      "<P>Copyright (C) 1998 - 2018 by <a href=\"mailto:artkoz78@gmail.com\">Artur Kozioł</a>"
	                      "<br>Copyright (C) 2020 - 2025 by <a href='mailto:nicegorov@yandex.ru'>Nick Egorrov</a>"
	                      "<h2>Sources and updates</h2>"
	                      "<p>Source code and new releases are available at the following addresses:"
	                      "<ul>"
	                      "<li><a href='https://github.com/GCodeProjects/GCodeWorkShop'>https://github.com/GCodeProjects/GCodeWorkShop</a></li>"
	                      "<li><a href='https://codeberg.org/u-235/GCodeWorkShop'>https://codeberg.org/u-235/GCodeWorkShop</a></li>"
	                      "<li><a href='https://gitflic.ru/project/u-235/gcodeworkshop'>https://gitflic.ru/project/u-235/gcodeworkshop</a></li>"
	                      "</ul>"
	                      "<h2>Thanks</h2>"
	                      "<P>Spanish translation, fixes and other help thanks to Otto Huamani"
	                      "<br />Catalan translation and deb package thanks to Jordi Sayol i Salomó"
	                      "<br />German translation and other fixes thanks to Michael Numberger"
	                      "<br />Czech translation thanks to Pavel Fric"
	                      "<br />Finnish translation thanks to Janne Mäntyharju"
	                      "<br />Dutch translation thanks to Henk van Andel"
	                      "<br />OS X patch and other updates thanks to Janne Mäntyharju"
	                      "<P>New EdytorNC icon thanks to Jakub Gajewski"
	                      "<P>GCodeWorkShop contains pieces of code from other Open Source projects."
	                      "<h2>License</h2>"
	                      "<P><i>GCodeWorkShop is free software; you can redistribute it and/or modify"
	                      "it under the terms of the GNU General Public License  as published by"
	                      "the Free Software Foundation; either version 2 of the License, or"
	                      "(at your option) any later version.</i>"
	                      "<P><i>The program is provided AS IS with NO WARRANTY OF ANY KIND,"
	                      "INCLUDING THE WARRANTY OF DESIGN,"
	                      "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</i>").arg(GCODEWORKSHOP_VERSION));
}

Document* GCodeWorkShop::createDocument(const QString& type)
{
	Document* doc = m_documentManager->createDocument(type, "");

	if (!doc) {
		return nullptr;
	}

	if (m_saveDirectory.isEmpty()) {
		doc->setPath(lastOpenedPath());
	} else {
		doc->setPath(m_saveDirectory);
	}

	if (m_MdiWidgetsMaximized) {
		doc->widget()->parentWidget()->showMaximized();
	} else {
		doc->widget()->parentWidget()->showNormal();
	}

	return doc;
}

void GCodeWorkShop::readSettings()
{
	QSettings& settings = *Medium::instance().settings();

	m_disableFileChangeMonitor = settings.value("DisableFileChangeMonitor",
	                             false).toBool();

	if (m_disableFileChangeMonitor) {
		fileChangeMonitor.clear();
	} else {
		fileChangeMonitor = new QFileSystemWatcher(this);
		connect(fileChangeMonitor, SIGNAL(fileChanged(const QString)), this,
		        SLOT(fileChanged(const QString)));
	}

	m_lastOpenedPath = settings.value("LastDir",  m_lastOpenedPath).toString();
	setFileFilter(settings.value("Extensions", m_extensions).toStringList());
	m_saveExtension = settings.value("DefaultSaveExtension", m_saveExtension).toString();
	m_saveDirectory = settings.value("DefaultSaveDirectory", m_saveDirectory).toString();
	m_startEmpty = settings.value("StartEmpty", false).toBool();
	m_defaultReadOnly = settings.value("ViewerMode", false).toBool();

	fileDialogState = settings.value("FileDialogState", QByteArray()).toByteArray();

#ifdef Q_OS_LINUX
	m_calcBinary = "kcalc";
#endif

#ifdef Q_OS_WIN32
	m_calcBinary = "calc.exe";
#endif

	m_calcBinary = settings.value("CalcBinary", m_calcBinary).toString();

	m_recentFiles->load(&settings);
	GCoderWidgetProperties::Ptr prop = m_documentManager->documentWidgetProperties(GCoder::DOCUMENT_TYPE);

	if (prop) {
		prop->load(&settings);
		m_documentManager->setDocumentWidgetProperties(prop);
	}

	GCoderStyle::Ptr style = m_documentManager->documentStyle(GCoder::DOCUMENT_TYPE);

	if (style) {
		style->load(&settings);
		m_documentManager->setDocumentStyle(style);
	}

	settings.beginGroup("GCode");
	settings.beginGroup("Converter");
	GCode::Converter::Options gcodeOptions = GCode::Converter::defaultOptions();
	gcodeOptions.load(&settings);
	GCode::Converter::setDefaultOptions(gcodeOptions);
	settings.endGroup();
	settings.endGroup();

	m_sessionManager->load(&settings);

	if (!m_startEmpty) {
		//		openFilesFromSession();
	}
}

void GCodeWorkShop::writeSettings()
{
	QSettings& settings = *Medium::instance().settings();

	settings.setValue("LastDir", m_lastOpenedPath);
	settings.setValue("Extensions", m_extensions);
	settings.setValue("DefaultSaveExtension", m_saveExtension);
	settings.setValue("DefaultSaveDirectory", m_saveDirectory);
	settings.setValue("CalcBinary", m_calcBinary);
	settings.setValue("ViewerMode", m_defaultReadOnly);
	settings.setValue("StartEmpty", m_startEmpty);
	settings.setValue("DisableFileChangeMonitor", m_disableFileChangeMonitor);
	settings.setValue("FileDialogState", fileDialogState);

	//cleanup old settings
	settings.remove("LastDoc");

	settings.beginGroup("GCode");
	settings.beginGroup("Converter");
	GCode::Converter::defaultOptions().save(&settings);
	settings.endGroup();
	settings.endGroup();

	if (!m_startEmpty) {
		storeFileInfoInSession();
	}
}

Document* GCodeWorkShop::activeDocument() const
{
	return m_documentManager->activeDocument();
}

GCoderDocument* GCodeWorkShop::activeGCoderDocument() const
{
	return dynamic_cast<GCoderDocument*>(activeDocument());
}

Document* GCodeWorkShop::findDocument(const QString& fileName)
{
	QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

	if (canonicalFilePath.isEmpty()) {
		canonicalFilePath = fileName;
	}

	return m_documentManager->findDocumentByFilePath(canonicalFilePath);
}

bool GCodeWorkShop::setActiveDocument(Document* doc)
{
	return m_documentManager->setActiveDocument(doc);
}

bool GCodeWorkShop::setActiveDocument(const QString& fileName)
{
	return m_documentManager->setActiveDocument(fileName);
}

QString GCodeWorkShop::currentPath() const
{
	Document* child = activeDocument();

	if (child) {
		return child->filePath();
	}

	return QDir::homePath();
}

QString GCodeWorkShop::lastOpenedPath() const
{
	return m_lastOpenedPath;
}

void GCodeWorkShop::setLastOpenedPath(const QString& path)
{
	m_lastOpenedPath = path;
}

void GCodeWorkShop::loadFile(const DocumentInfo::Ptr& info, bool checkAlreadyLoaded)
{
	QFileInfo file(info->filePath);

	if (!file.exists() || !file.isFile() || !file.isReadable()) {
		return;
	}

	info->filePath = file.canonicalFilePath();

	if (checkAlreadyLoaded && setActiveDocument(info->filePath)) {
		return;
	}

	Document* doc = createDocument(info->documentType());

	if (!doc) {
		return;
	}

	doc->setDocumentInfo(info);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	bool status = doc->load();
	QApplication::restoreOverrideCursor();

	if (status) {
		setLastOpenedPath(file.path());
		m_mainWindow->updateStatusBar();
		m_recentFiles->add(info->filePath);
		fireCurrentDirChanged();
	} else {
		QMessageBox::warning(mainWindow(), tr("GCodeWorkShop"), tr("Cannot read file \"%1\".\n %2")
		                     .arg(doc->filePath()).arg(doc->ioErrorString()));
		doc->close();
	}
}

void GCodeWorkShop::recentFilesChanged()
{
	m_recentFiles->save(Medium::instance().settings());
}

void GCodeWorkShop::fileOpenRecent(QAction* act)
{
	openFile(act->data().toString());
}

void GCodeWorkShop::messReceived(const QString& text)
{
	QString str = text;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
	auto behavior = QString::SkipEmptyParts;
#else
	auto behavior = Qt::SkipEmptyParts;
#endif
	QStringList list1 = str.split(";", behavior);

	for (int i = 0; i < list1.size(); ++i) {
		openFile(list1.at(i));
	}

	emit needToShow();
}

void GCodeWorkShop::createUserToolTipsFile()
{
	QString fileName;

	if (activeDocument()) {
		fileName = activeDocument()->path();
	} else {
		return;
	}

	fileName += "/cnc_tips.txt";

	if (QFileInfo(fileName).exists()) {
		openFile(fileName);
	} else {
		QFile file(fileName);

		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
			return;
		}

		// qDebug() << fileName;

		QTextStream out(&file);
		out << "# " << fileName << "\n" << "\n";
		out << "#+++++++++++++++++++++++++++++++++\n";
		out << tr("# ++++++ EXAMPLE ++++++") << "\n";
		out << "# [OKUMA]" << "\n";
		out << tr("# M00=\"<b>M00</b> - program stop, unconditional\"") << "\n";
		out << "#+++++++++++++++++++++++++++++++++\n\n";

		out << "[OKUMA]" << "\n" << "\n" << "\n";
		out << "[FANUC]" << "\n" << "\n" << "\n";
		out << "[SINUMERIK]" << "\n" << "\n" << "\n";
		out << "[SINUMERIK_840]" << "\n" << "\n" << "\n";
		out << "[PHILIPS]" << "\n" << "\n" << "\n";
		out << "[HEIDENHAIN]" << "\n" << "\n" << "\n";
		out << "[HEIDENHAIN_ISO]" << "\n" << "\n" << "\n";
		file.close();
		openFile(fileName);
	}

	setActiveDocument(fileName);
}

void GCodeWorkShop::createGlobalToolTipsFile()
{
	QString fileName = writeTooltipFile();


	if (QFileInfo(fileName).exists()) {
		openFile(fileName);
	}

	setActiveDocument(fileName);
}

void GCodeWorkShop::attachHighlighterToDirButtonClicked(bool attach)
{
	QFile file;

	bool hasMdiChild = (activeDocument() != 0);

	if (hasMdiChild) {
		QDir dir = activeDocument()->dir();
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		dir.setSorting(QDir::Name);
		dir.setNameFilters(QStringList("*.cfg"));

		QFileInfoList list = dir.entryInfoList();

		for (const QFileInfo& fileInfo : list) {
			file.setFileName(fileInfo.absoluteFilePath());
			file.remove();
		}

		if (attach) {
			file.setFileName(activeDocument()->path() + "/" + m_mainWindow->highlightComboBox()->currentText() + ".cfg");
			file.open(QIODevice::ReadWrite);
			file.close();;
		}
	}
}

void GCodeWorkShop::attachHighlightToDirActClicked()
{
	attachHighlighterToDirButtonClicked(true);
}

void GCodeWorkShop::deAttachHighlightToDirActClicked()
{
	attachHighlighterToDirButtonClicked(false);
}

int GCodeWorkShop::defaultHighlightMode(const QString& filePath)
{
	QDir dir;
	bool ok;

	dir.setPath(filePath);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	dir.setNameFilters(QStringList("*.cfg"));

	QFileInfoList list = dir.entryInfoList();

	if (!list.isEmpty()) {
		QFileInfo name = list.at(0);
		int id = m_mainWindow->highlightComboBox()->findText(name.baseName());

		if (id >= 0) {
			//highlightTypeCombo->setCurrentIndex(id);
			return (m_mainWindow->highlightComboBox()->itemData(id).toInt(&ok));
		}
	}

	return MODE_AUTO;
}

void GCodeWorkShop::fireCurrentDirChanged()
{
	QString path;

	if (m_mainWindow->isPanelHidden()) { //if((!isVisible()) || panelHidden)
		return;
	}

	if ((activeDocument() != nullptr) && !activeDocument()->isUntitled()) {
		path = activeDocument()->filePath();

		if (QFileInfo(path).exists()) {
			path = QFileInfo(path).canonicalPath();
		} else {
			path = path.remove(QFileInfo(path).fileName());
		}
	} else {
		path = lastOpenedPath();
	}

	if (path.isEmpty()) {
		return;
	}

	emit currentDirChanged(path);
}

void GCodeWorkShop::sessionsChanged()
{
	m_sessionManager->save(Medium::instance().settings());
}

void GCodeWorkShop::beforeCurrentSessionChanged()
{
	storeFileInfoInSession();
}

void GCodeWorkShop::currentSessionChanged()
{
	m_mainWindow->closeAllMdiWindows();
	openFilesFromSession();
	statusBarMessage(tr("Session %1 loaded").arg(m_sessionManager->currentSession()), 5000);
}

void GCodeWorkShop::changeSession(const QString& name)
{
	m_sessionManager->setCurrentSession(name);
}

void GCodeWorkShop::openFilesFromSession()
{
	for (const DocumentInfo::Ptr& info : m_sessionManager->documentInfoList()) {
		m_MdiWidgetsMaximized = false;
		loadFile(info, false);
	}
}

void GCodeWorkShop::storeFileInfoInSession()
{
	QList<DocumentInfo::Ptr> infoList;

	for (const Document* doc : m_documentManager->documentList()) {
		DocumentInfo::Ptr info = doc->documentInfo();
		infoList.append(info);
	}

	m_sessionManager->setDocumentInfoList(infoList);
}

void GCodeWorkShop::showSessionDialog()
{
	GUI::SessionDialog* sesDialog = new GUI::SessionDialog(mainWindow(), m_sessionManager);
	sesDialog->exec();
	sesDialog->deleteLater();
}

void GCodeWorkShop::savePrinterSettings(QPrinter* printer)
{
#ifndef QT_NO_PRINTER

	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("PrinterSettings");

	settings.setValue("PrinterName", printer->printerName());
	settings.setValue("CollateCopies", printer->collateCopies());
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
	settings.setValue("Orientation", printer->orientation());
#else
	settings.setValue("Orientation", printer->pageLayout().orientation());
#endif
	settings.setValue("ColorMode", printer->colorMode());
	QPageLayout layout = printer->pageLayout();
	settings.setValue("PageSize", layout.pageSize().id());
	settings.setValue("Duplex", printer->duplex());
	//settings.setValue("Resolution", printer->resolution());

	settings.endGroup();

#endif
}

void GCodeWorkShop::loadPrinterSettings(QPrinter* printer)
{
#ifndef QT_NO_PRINTER

	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("PrinterSettings");

	printer->setPrinterName(settings.value("PrinterName").toString());
	printer->setCollateCopies(settings.value("CollateCopies").toBool());
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
	printer->setOrientation((QPrinter::Orientation)settings.value("Orientation").toInt());
#else
	printer->pageLayout().setOrientation((QPageLayout::Orientation)
	                                     settings.value("Orientation").toInt());
#endif
	printer->setColorMode((QPrinter::ColorMode)settings.value("ColorMode").toInt());

	QPageLayout layout = printer->pageLayout();
	layout.setPageSize(QPageSize((QPageSize::PageSizeId)settings.value("PageSize",
	                             (int)QPageSize::A4).toInt()));
	printer->setPageLayout(layout);

	//printer->setPaperSize((QPrinter::PaperSize)settings.value("PageSize", (int)QPrinter::A4).toInt());
	printer->setDuplex((QPrinter::DuplexMode)settings.value("Duplex").toInt());
	//printer->setResolution(settings.value("Resolution").toInt());

	settings.endGroup();

#endif
}

void GCodeWorkShop::fileChanged(const QString& fileName)
{
	Document* doc = findDocument(fileName);
	bool modified = false;

	if (doc) {
		modified = doc->isModified();
		setActiveDocument(doc);
	} else {
		fileChangeMonitor->removePath(fileName);
		return;
	}

	fileChangeMonitor->addPath(fileName);

	QMessageBox msgBox;
	msgBox.setParent(mainWindow(), Qt::Dialog);
	msgBox.setText(tr("File \"%1\" <b>was modified on disk.</b><p>Do you want to reload it?</p>%2")
	               .arg(fileName)
	               .arg((modified ? tr("<p><b>Warning:</b> File in editor contains unsaved changes.</p>") : "")));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setIcon(QMessageBox::Warning);

	int ret = msgBox.exec();

	switch (ret) {
	case QMessageBox::Yes:
		QApplication::setOverrideCursor(Qt::WaitCursor);
		doc->load();
		QApplication::restoreOverrideCursor();
		break;

	case QMessageBox::No:
		break;

	default:
		break;
	}
}

void GCodeWorkShop::startSerialPortServer()
{
	QString path = QDir::toNativeSeparators(QApplication::applicationDirPath() + "/");
	QString fileName;

#ifdef Q_OS_WIN32
	fileName = "gcodefileserver.exe";
#else
	fileName = "gcodefileserver";
#endif

	QProcess::startDetached(path + fileName, QStringList(), path);
}

void GCodeWorkShop::customContextMenuRequest(Document* doc, const QPoint& pos)
{
	QMenu* menu = nullptr;

	if (!menu) {
		GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(doc);

		if (gdoc) {
			menu = doContextMenuGCoder(gdoc, pos);
		}
	}

	if (menu) {
		menu->exec(doc->widget()->mapToGlobal(pos));
		menu->deleteLater();
	}
}

QMenu* GCodeWorkShop::doContextMenuGCoder(GCoderDocument* doc, const QPoint& pos)
{
	QMenu* menu = doc->createStandardContextMenu(pos);
	menu->addSeparator();
	menu->addAction(m_mainWindow->addonsActions()->semiComment());
	menu->addAction(m_mainWindow->addonsActions()->paraComment());
	menu->addSeparator();
	menu->addAction(m_mainWindow->addonsActions()->blockSkipIncrement());
	menu->addAction(m_mainWindow->addonsActions()->blockSkipDecrement());
	menu->addAction(m_mainWindow->addonsActions()->blockSkipRemove());
	menu->addSeparator();
	menu->addAction(m_mainWindow->toolActions()->inLineCalc());
	return menu;
}

void GCodeWorkShop::doShowInLineCalc()
{
	GCoderDocument* gdoc = activeGCoderDocument();

	if (gdoc) {
		gdoc->showInLineCalc();
	}
}

void GCodeWorkShop::watchFile(const QString& fileName, bool add)
{
	if (fileChangeMonitor) {
		bool exists = fileChangeMonitor->files().contains(fileName);

		if (add) {
			if (!exists) {
				fileChangeMonitor->addPath(fileName);
			}
		} else {
			if (exists) {
				fileChangeMonitor->removePath(fileName);
			}
		}
	}
}

void GCodeWorkShop::statusBarMessage(const QString& msg, int timeout)
{
	mainWindow()->statusBar()->showMessage(msg, timeout);
}
