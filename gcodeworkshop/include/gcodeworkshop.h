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

#ifndef GCODEWORKSHOP_H
#define GCODEWORKSHOP_H

#include <QByteArray>   // for QByteArray
// Special for the moc_gcodeworkshop.cpp
#include <QKeySequence> // IWYU pragma: keep
#include <QObject>      // for slots, Q_OBJECT, signals
#include <QPointer>     // for QPointer
#include <QString>      // for QString
#include <QStringList>  // for QStringList

class QDir;
class QFileSystemWatcher;
template <class Key, class T> class QMap;
class QMainWindow;
class QMdiSubWindow;
class QMenu;
class QPoint;
class QPrinter;
class QProcess;
class QSettings;

#include <documentinfo.h>           // for DocumentInfo, DocumentInfo::Ptr
#include <utils/keysequencemap.h>   // for KeySequenceMap

class Document;
class DocumentManager;
class GCodeFileServer;
class GCoderDocument;
class Medium;
class RecentFiles;
class SessionManager;

namespace GUI {
namespace WorkShop {
class MainWindow;
} // namespace WorkShop
} // namespace GUI


class GCodeWorkShop : public QObject
{
	Q_OBJECT

public:
	static GCodeWorkShop* instance();

	~GCodeWorkShop();

	void launch(const QStringList& args);

	QMainWindow* mainWindow();
	DocumentManager* documentManager() const;
	Document* activeDocument() const;
	GCoderDocument* activeGCoderDocument() const;
	QString currentPath() const;
	QString lastOpenedPath() const;

signals:
	void updateTranslations();
	void updateIcons();
	void updateShortcuts(const QMap<QString, QKeySequence>&);
	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg);
	void fileFilterChanged(const QStringList& extensions);
	void currentDirChanged(const QString& path);

protected:
	static GCodeWorkShop* SINGLETON;
	GUI::WorkShop::MainWindow* m_mainWindow;

	GCodeWorkShop(Medium* medium);

	Medium* mMedium;
	KeySequenceMap m_shortcuts;

public slots:
	void openFile(const QString& fileName, const QString& mime = QString());
	void diffTwoFiles(const QString& filename1, const QString& filename2);

	void messReceived(const QString& text = "");
	Document* newFile();
	bool maybeSave(Document* doc);
	bool maybeSaveAll();

	bool close();

protected:
	void setLastOpenedPath(const QString& path);

	bool saveDocument(Document* doc, bool forceSaveAs);

public:
	QStringList fileFilter() const;

protected:
	void setFileFilter(const QStringList& extensions);

public slots:
	Document* newFileFromTemplate();

private slots:
	void openDir(const QDir& dir);

public slots:
	void open();
	void openExample();
	bool save();
	bool saveAll();
	bool saveAs();
	void printFile();
	void filePrintPreview();
	void printPreview(QPrinter* printer);
	void cut();
	void undo();
	void redo();
	void copy();
	void paste();
	void about();

private slots:
	Document* createDocument(const QString& type);

public slots:
	bool setActiveDocument(Document* doc);
	bool setActiveDocument(const QString& fileName);

private slots:
	void loadFile(const DocumentInfo::Ptr& options, bool checkAlreadyLoaded = true);
	void recentFilesChanged();

public slots:
	void activeWindowChanged(QMdiSubWindow* window);
	void deleteText();
	void findInFl();
	void selAll();
	void config();
	void readOnly();
	void doCalc();
	void showFindToolBar();
	void showReplaceToolBar();

	void createSerialToolBar();

	void startSerialPortServer();
	void attachSerialConfigToDir(const QString& name, bool attach);
	void attachHighlighterToDir(bool attach);
	void createGlobalToolTipsFile();
	void createUserToolTipsFile();

	void doDiffL();
	void doDiffR();
	void doDiff();
	void diffEditorFile();

	void closeCurrentWindow();
	void closeAllMdiWindows();

	void goToLine(const QString& fileName, int line);

private slots:
	void sessionsChanged();
	void beforeCurrentSessionChanged();
	void currentSessionChanged();

public slots:
	void changeSession(const QString& name);
	void showSessionDialog();

private slots:
	void fileChanged(const QString& fileName);

public slots:
	void tileSubWindowsHorizontally();
	void tileSubWindowsVertycally();
	void cascadeSubWindows();
	void activateNextSubWindow();
	void activatePreviousSubWindow();

private slots:
	void customContextMenuRequest(Document* doc, const QPoint& pos);
	QMenu* doContextMenuGCoder(GCoderDocument* doc, const QPoint& pos);

public slots:
	void doShowInLineCalc();

private slots:
	void watchFile(const QString& fileName, bool add);

signals:
	void needToShow();
	void intCapsLockChanged(bool enable);

private:
	void readSettings();
	void writeSettings();
	int defaultHighlightMode(const QString& filePath);
	void fireCurrentDirChanged();
	Document* findDocument(const QString& fileName);
	void createDiffApp();
	void openFilesFromSession();
	void storeFileInfoInSession();
	void savePrinterSettings(QPrinter* printer);
	void loadPrinterSettings(QPrinter* printer);
	void statusBarMessage(const QString& msg, int timeout);

	SessionManager* m_sessionManager;

	bool m_MdiWidgetsMaximized;

	DocumentManager* m_documentManager;
	QString m_calcBinary;
	QStringList m_extensions;
	QString m_saveExtension;
	QString m_saveDirectory;
	QString m_lastOpenedPath;
	bool m_defaultReadOnly;
	bool m_startEmpty;
	bool m_disableFileChangeMonitor;
	bool m_findInFilesHighlightEnable;

	QByteArray fileDialogState;

	RecentFiles* m_recentFiles;

	QProcess* proc;
	QProcess* sfsProc;

	QPointer<QFileSystemWatcher> fileChangeMonitor;
};

#endif // GCODEWORKSHOP_H
