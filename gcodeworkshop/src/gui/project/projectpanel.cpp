/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2026 Nick Egorrov, nicegorov@yandex.ru
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

#include <QAbstractItemModel>   // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic
#include <QDir>                 // for QDir
#include <QFileDialog>          // for QFileDialog
#include <QFileIconProvider>    // for QFileIconProvider
#include <QFileInfo>            // for QFileInfo
#include <QFileSystemModel>     // for QFileSystemModel
#include <QIcon>                // for QIcon
#include <QItemSelectionModel>  // for QItemSelectionModel
#include <QList>                // for QList
#include <QMainWindow>          // for QMainWindow
#include <QMessageBox>          // for QMessageBox, operator|
#include <QModelIndex>          // for QModelIndex
#include <QModelIndexList>      // for QModelIndexList
#include <QSettings>            // for QSettings
#include <QStandardItemModel>   // for QStandardItem, QStandardItemModel
#include <QStatusBar>           // for QStatusBar
#include <QStringList>          // for QStringList
#include <QToolButton>          // for QToolButton
#include <QTreeView>            // for QTreeView
#include <QVariant>             // for QVariant
#include <Qt>                   // for MatchFlag, operator|, WindowType
#include <QtGlobal>             // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QFlags

#include <gcodeworkshop.h>

#include "projectpanel.h"
#include "ui_projectpanel.h"


#define CFG_SECTION         "ProjectPanel"
#define CFG_KEY_FILE_PATH   "FilePath"


GUI::ProjectPanel::ProjectPanel(QWidget* parent)
	: QWidget{parent},
	  m_projectItem(nullptr),
	  m_modified(false),
	  m_filePath(),
	  m_model(new QStandardItemModel()),
	  ui(new Ui::ProjectPanel())
{
	ui->setupUi(this);
	ui->treeView->setModel(m_model);
	GUI::ProjectPanel::loadIcons();

	connect(ui->treeView, &QTreeView::doubleClicked, this, &ProjectPanel::treeViewDoubleClicked);
	connect(ui->btn_newProject, &QToolButton::clicked, this, &ProjectPanel::newProject);
	connect(ui->btn_saveProject, &QToolButton::clicked, this, &ProjectPanel::saveProject);
	connect(ui->btn_saveProjectAs, &QToolButton::clicked, this, &ProjectPanel::saveProjectAs);
	connect(ui->btn_openProject, &QToolButton::clicked, this, &ProjectPanel::openProject);
	connect(ui->btn_addItem, &QToolButton::clicked, this, &ProjectPanel::addItem);
	connect(ui->btn_removeItem, &QToolButton::clicked, this, &ProjectPanel::removeItem);
}

bool GUI::ProjectPanel::maybeSave()
{
	if (m_modified) {
		QMessageBox msgBox;
		msgBox.setParent(GCodeWorkShop::instance()->mainWindow(), Qt::Dialog);
		msgBox.setText(tr("<b>Project: \"%1\"\n has been modified.</b>").arg(m_filePath));
		msgBox.setInformativeText(tr("Do you want to save your changes ?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		msgBox.setIcon(QMessageBox::Warning);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			saveProject();
			return true;
			break;

		case QMessageBox::Discard:
			m_modified = false;
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

void GUI::ProjectPanel::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::ProjectPanel::loadIcons()
{
	ui->btn_newProject->setIcon(QIcon(":/images/project_new.png"));
	ui->btn_openProject->setIcon(QIcon(":/images/project_open.png"));
	ui->btn_saveProject->setIcon(QIcon(":/images/project_save.png"));
	ui->btn_saveProjectAs->setIcon(QIcon(":/images/project_save_as.png"));
	ui->btn_addItem->setIcon(QIcon(":/images/project_add.png"));
	ui->btn_removeItem->setIcon(QIcon(":/images/project_remove.png"));
}

void GUI::ProjectPanel::loadSettings(QSettings* cfg)
{
	if (cfg->childGroups().contains(CFG_SECTION)) {
		cfg->beginGroup(CFG_SECTION);
		m_filePath = cfg->value(CFG_KEY_FILE_PATH, "").toString();
		cfg->endGroup();
	} else {
		m_filePath = cfg->value("CurrentProjectName", "").toString();
	}

	load(m_filePath);
}

void GUI::ProjectPanel::saveSettings(QSettings* cfg) const
{
	cfg->beginGroup(CFG_SECTION);
	cfg->setValue(CFG_KEY_FILE_PATH, m_filePath);
	cfg->endGroup();

	// Remove the old settings
	cfg->remove("CurrentProjectName");
}

void GUI::ProjectPanel::newProject()
{
	if (!maybeSave()) {
		return;
	}

	QString fileName = selectName();

	if (fileName.isEmpty()) {
		return;
	}

	m_filePath = fileName;

	QStandardItem* parentItem = m_model->invisibleRootItem();
	QStandardItem* item = new QStandardItem(QIcon(":/images/edytornc.png"),
	                                        QFileInfo(m_filePath).fileName());

	parentItem->appendRow(item);

	m_projectItem = item;
	m_modified = true;
}

void GUI::ProjectPanel::openProject()
{
	if (!maybeSave()) {
		return;
	}

	QString filters = tr("GCodeWorkShop project file (*.ncp)");
	QString fileName = QFileDialog::getOpenFileName(
	                       GCodeWorkShop::instance()->mainWindow(),
	                       tr("Open the project file..."),
	                       m_filePath,
	                       filters);

	if (fileName.isEmpty()) {
		return;
	}

	load(fileName);
}

void GUI::ProjectPanel::saveProject()
{
	QString path, fileName;
	int fileCount;
	QStandardItem* item;


	if (m_filePath.isEmpty() || m_projectItem == nullptr) {
		return;
	}

	QSettings settings(m_filePath, QSettings::IniFormat);

	settings.remove("ProjectFiles");
	settings.beginWriteArray("ProjectFiles");

	fileCount = 0;

	for (int i = 0; i < m_projectItem->rowCount(); i++) {
		item = m_projectItem->child(i, 0);
		path = item->text();

		for (int j = 0; j < item->rowCount(); j++) {
			fileName = item->child(j, 0)->text();
			settings.setArrayIndex(fileCount);
			settings.setValue("File", QFileInfo(path, fileName).absoluteFilePath());
			fileCount++;
		}
	}

	settings.endArray();

	if (settings.status() == QSettings::NoError) {
		m_modified = false;
		GCodeWorkShop::instance()->mainWindow()->statusBar()->showMessage(tr("Project saved"), 5000);
	}
}

void GUI::ProjectPanel::saveProjectAs()
{
	QString fileName = selectName();

	if (fileName.isEmpty()) {
		return;
	}

	m_filePath = fileName;
	QStandardItem* parentItem = m_model->invisibleRootItem();
	parentItem->child(0, 0)->setText(QFileInfo(m_filePath).fileName());
	parentItem->child(0, 0)->setToolTip(QDir::toNativeSeparators(QFileInfo(
	                                        m_filePath).absoluteFilePath()));
	saveProject();
}

void GUI::ProjectPanel::addItem()
{
	QFileInfo file;
	QStandardItem* item;
	QIcon icon;

	if (m_projectItem == nullptr) {
		return;
	}

	QString filters = tr("All files (*.* *);;"
	                     "CNC programs files (*.nc);;"
	                     "CNC programs files (*.nc *.ngc *.min *.anc *.cnc);;"
	                     "Documents (*.odf *.odt *.pdf *.doc *.docx  *.xls *.xlsx);;"
	                     "Drawings (*.dwg *.dxf);;"
	                     "Pictures (*.jpg *.bmp *.svg);;"
	                     "Text files (*.txt)");
	QStringList fileList = QFileDialog::getOpenFileNames(
	                           GCodeWorkShop::instance()->mainWindow(),
	                           tr("Add files to project"),
	                           GCodeWorkShop::instance()->lastOpenedPath(),
	                           filters, 0);

	if (fileList.isEmpty()) {
		return;
	}

	QStringList::Iterator it = fileList.begin();

	QStandardItem* parentItem = m_projectItem;

	if (it != fileList.end()) {
		file.setFile(*it);

		if ((file.absoluteDir().exists()) && (file.absoluteDir().isReadable())) {

			QList<QStandardItem*> items = m_model->findItems(QDir::toNativeSeparators(
			                                  file.absoluteDir().canonicalPath()),
			                              Qt::MatchFixedString | Qt::MatchCaseSensitive | Qt::MatchRecursive, 0);

			if (!items.isEmpty()) {
				item = items.at(0);

				if (item->text() != file.absoluteDir().canonicalPath()) {
					item = new QStandardItem(QIcon(":/images/folder.png"),
					                         QDir::toNativeSeparators(file.absoluteDir().canonicalPath()));
					parentItem->appendRow(item);
				}
			} else {
				item = new QStandardItem(QIcon(":/images/folder.png"),
				                         QDir::toNativeSeparators(file.absoluteDir().canonicalPath()));
				parentItem->appendRow(item);
			}

			parentItem = item;
		} else {
			return;
		}
	}

	QFileSystemModel* fModel = new QFileSystemModel;

	while (it != fileList.end()) {
		file.setFile(*it);

		if ((file.exists()) && (file.isReadable())) {
			icon = fModel->iconProvider()->icon(file);

			if (icon.isNull()) {
				icon = QIcon(":/images/ncfile.png");
			}

			item = new QStandardItem(icon, file.fileName());
			parentItem->appendRow(item);
		}

		++it;
	}

	fModel->deleteLater();
	ui->treeView->expandAll(); //model->indexFromItem(currentProject));
	m_modified = true;
	GCodeWorkShop::instance()->mainWindow()->statusBar()->showMessage(tr("Project opened"), 5000);
}

void GUI::ProjectPanel::removeItem()
{
	const QModelIndexList& list = ui->treeView->selectionModel()->selectedIndexes();

	for (QModelIndex it : list) {
		QStandardItem* item = m_model->itemFromIndex(it);

		if (item == nullptr) {
			return;
		}

		if (item == m_projectItem) {
			continue;
		}

		if (!item->hasChildren()) {
			m_modified = m_model->removeRow(item->row(), m_model->indexFromItem(item->parent()));
		}
	}
}

void GUI::ProjectPanel::load(const QString& projectName)
{
	QFileInfo file;
	QIcon icon;


	if (projectName.isEmpty()) {
		return;
	}

	m_filePath = projectName;

	m_model->clear();

	QSettings settings(m_filePath, QSettings::IniFormat);

	QStandardItem* item = new QStandardItem(QIcon(":/images/edytornc.png"),
	                                        QFileInfo(m_filePath).fileName());
	item->setToolTip(QDir::toNativeSeparators(m_filePath));

	m_model->invisibleRootItem()->appendRow(item);

	m_projectItem = item;

	QFileSystemModel* fModel = new QFileSystemModel;

	int max = settings.beginReadArray("ProjectFiles");

	for (int i = 0; i < max; ++i) {
		settings.setArrayIndex(i);
		file.setFile(settings.value("File", "").toString());

		if ((file.absoluteDir().exists()) && (file.absoluteDir().isReadable())) {
			QList<QStandardItem*> items = m_model->findItems(file.absoluteDir().canonicalPath(),
			                              Qt::MatchFixedString | Qt::MatchCaseSensitive | Qt::MatchRecursive, 0);

			if (!items.isEmpty()) {
				item = items.at(0);

				if (item->text() != file.absoluteDir().canonicalPath()) {
					item = new QStandardItem(QIcon(":/images/folder.png"), file.absoluteDir().canonicalPath());
					item->setToolTip(QDir::toNativeSeparators(file.absoluteDir().canonicalPath()));
					m_projectItem->appendRow(item);
				}
			} else {
				item = new QStandardItem(QIcon(":/images/folder.png"), file.absoluteDir().canonicalPath());
				item->setToolTip(QDir::toNativeSeparators(file.absoluteDir().canonicalPath()));
				m_projectItem->appendRow(item);
			}

			if ((file.exists()) && (file.isReadable())) {
				icon = fModel->iconProvider()->icon(file);

				if (icon.isNull()) {
					icon = QIcon(":/images/ncfile.png");
				}

				QStandardItem* childItem = new QStandardItem(icon, file.fileName());
				childItem->setToolTip(file.fileName());
				item->appendRow(childItem);
			}
		}
	}

	settings.endArray();
	fModel->deleteLater();
	ui->treeView->expandAll();
	m_modified = false;
}

void GUI::ProjectPanel::treeViewDoubleClicked(const QModelIndex& index)
{
	QFileInfo file;

	if ((!index.isValid())) {
		return;
	}

	QStandardItem* item = m_model->itemFromIndex(index);

	if (item == nullptr || item->parent() == nullptr) {
		return;
	}

	if (item->hasChildren()) {
		return;
	}

	file.setFile(item->parent()->text(), item->text());

	if ((file.exists()) && (file.isReadable())) {
		emit fileClicked(file.filePath(), "");
	}
}

QString GUI::ProjectPanel::selectName()
{
	QString filters = tr("GCodeWorkShop project file (*.ncp)");
	QString file = QFileDialog::getSaveFileName(
	                   GCodeWorkShop::instance()->mainWindow(),
	                   tr("Select the project name and location..."),
	                   m_filePath,
	                   filters);

	return file;
}
