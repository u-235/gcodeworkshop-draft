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

#include <QByteArray>           // for QByteArray
#include <QCheckBox>            // for QCheckBox
#include <QDir>                 // for QDir, operator|
#include <QEvent>               // for QEvent
#include <QFile>                // for QFile
#include <QFileInfo>            // for QFileInfo
#include <QFileSystemModel>     // for QFileSystemModel
#include <QHeaderView>          // for QHeaderView
#include <QHelpEvent>           // for QHelpEvent
#include <QIODevice>            // for QIODevice
#include <QModelIndex>          // for QModelIndex
#include <QPoint>               // for QPoint
#include <QRect>                // for QRect
#include <QSettings>            // for QSettings
#include <QSpinBox>             // for QSpinBox
#include <QToolTip>             // for QToolTip
#include <QTreeView>            // for QTreeView
#include <QVariant>             // for QVariant
#include <Qt>                   // for CaseSensitivity
#include <QtGlobal>             // for QFlags, qint64

#include "filebrowserpanel.h"
#include "ui_filebrowserpanel.h" // for Ui::FileBrowserPanel


#define CFG_SECTION                     "FileBrowserPanel"
#define CFG_KEY_VIEW_STATE              "ViewState"
#define CFG_KEY_FILTER                  "Filter"
#define CFG_KEY_FOLLOW_CURRENT_DIR      "FollowCurrentDir"
#define CFG_KEY_FILE_PREVIEW_LINE_COUNT "FilePreviewNo"


GUI::FileBrowserPanel::FileBrowserPanel(QWidget* parent) :
	QWidget{parent},
	m_dirModel(new QFileSystemModel()),
	m_filters(),
	m_currentDir(QDir::currentPath()),
	ui(new Ui::FileBrowserPanel())
{
	ui->setupUi(this);

	m_dirModel->setResolveSymlinks(true);
	m_dirModel->setNameFilters(m_filters);
	m_dirModel->setNameFilterDisables(!ui->filterCheckBox->isChecked());
	m_dirModel->setFilter(QDir::Files | QDir::AllDirs | QDir::Drives | QDir::NoDot);
	ui->fileTreeView->installEventFilter(this);
	ui->fileTreeView->setModel(m_dirModel);
	ui->fileTreeView->setItemsExpandable(false);
	ui->fileTreeView->setSortingEnabled(true);
	ui->fileTreeView->header()->setSectionResizeMode(0, QHeaderView::Interactive);
	ui->fileTreeView->setColumnHidden(2, true);
	setRootPath(QDir::currentPath());

	connect(ui->fileTreeView, &QTreeView::doubleClicked, this, &GUI::FileBrowserPanel::fileTreeViewDoubleClicked);
	connect(ui->followCheckBox, &QCheckBox::toggled, this, &GUI::FileBrowserPanel::followCheckBoxToggled);
	connect(ui->filterCheckBox, &QCheckBox::toggled, this, &GUI::FileBrowserPanel::filterCheckBoxToggled);
}

GUI::FileBrowserPanel::~FileBrowserPanel()
{
	delete ui;
}

void GUI::FileBrowserPanel::setRootPath(const QString& path)
{
	m_dirModel->setRootPath(path);
	ui->fileTreeView->setRootIndex(m_dirModel->index(path));
	ui->fileTreeView->setToolTip(path);
}

void GUI::FileBrowserPanel::setNameFilters(const QStringList& filters)
{
	m_filters = filters;
	m_dirModel->setNameFilters(m_filters);
}

void GUI::FileBrowserPanel::currentDirChanged(const QString& path)
{
	if (path.isEmpty()) {
		return;
	}

	m_currentDir = path;

	if (ui->followCheckBox->isChecked()) {
		setRootPath(path);
	}
}

void GUI::FileBrowserPanel::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::FileBrowserPanel::loadSettings(QSettings* cfg)
{
	if (cfg->childGroups().contains(CFG_SECTION)) {
		cfg->beginGroup(CFG_SECTION);
		ui->fileTreeView->header()->restoreState(cfg->value(CFG_KEY_VIEW_STATE, QByteArray()).toByteArray());
		ui->filterCheckBox->setChecked(cfg->value(CFG_KEY_FILTER, false).toBool());
		ui->followCheckBox->setChecked(cfg->value(CFG_KEY_FOLLOW_CURRENT_DIR, false).toBool());
		ui->filePreviewSpinBox->setValue(cfg->value(CFG_KEY_FILE_PREVIEW_LINE_COUNT, 10).toInt());
		cfg->endGroup();
	} else {
		ui->fileTreeView->header()->restoreState(cfg->value("FileTreeViewState", QByteArray()).toByteArray());
		ui->followCheckBox->setChecked(cfg->value("FileBrowserShowCurrentFileDir", false).toBool());
		ui->filePreviewSpinBox->setValue(cfg->value("FilePreviewNo", 10).toInt());
	}

	ui->fileTreeView->setColumnHidden(2, true);
}

void GUI::FileBrowserPanel::saveSettings(QSettings* cfg) const
{
	cfg->beginGroup(CFG_SECTION);
	cfg->setValue(CFG_KEY_VIEW_STATE, ui->fileTreeView->header()->saveState());
	cfg->setValue(CFG_KEY_FILTER, ui->filterCheckBox->isChecked());
	cfg->setValue(CFG_KEY_FOLLOW_CURRENT_DIR, ui->followCheckBox->isChecked());
	cfg->setValue(CFG_KEY_FILE_PREVIEW_LINE_COUNT, ui->filePreviewSpinBox->value());
	cfg->endGroup();

	// Remove the old settings
	cfg->remove("FileTreeViewState");
	cfg->remove("FileBrowserShowCurrentFileDir");
	cfg->remove("FilePreviewNo");
}

void GUI::FileBrowserPanel::fileTreeViewDoubleClicked(const QModelIndex& index)
{
	QFileInfo info;

	if (!index.isValid()) {
		return;
	}

	QString path = m_dirModel->filePath(index);
	info.setFile(m_dirModel->filePath(index));

	if ((info.exists()) && (info.isReadable())) {
		if (info.isDir()) {
			if (path.endsWith("..")) {
				int idx = path.lastIndexOf('/');

				if (idx > 0) {
					idx = path.lastIndexOf('/', idx - 1);

					if (idx > 0) {
						path.remove(idx, (path.length() - idx));
					}
				}
			}

			setRootPath(path);
		} else {
			emit fileClicked(info.canonicalFilePath(), QString());
		}
	}
}

void GUI::FileBrowserPanel::followCheckBoxToggled(bool checked)
{
	if (checked) {
		setRootPath(m_currentDir);
	}
}

void GUI::FileBrowserPanel::filterCheckBoxToggled(bool checked)
{
	m_dirModel->setNameFilterDisables(!checked);
}

bool GUI::FileBrowserPanel::eventFilter(QObject* obj, QEvent* event)
{
	QString key, text;
	QModelIndex index;
	QFile file;
	QString fileName;

	QTreeView* treeView = dynamic_cast<QTreeView*>(obj);

	if (treeView != nullptr && event->type() == QEvent::ToolTip) {
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

		QPoint pos = treeView->viewport()->mapFromGlobal(helpEvent->globalPos());

		index = treeView->indexAt(pos);

		if (!index.isValid()) {
			return true;
		}

		fileName = m_dirModel->filePath(index);
		QFileInfo info{fileName};

		if (!m_filters.contains("*." + info.suffix(), Qt::CaseInsensitive)) {
			return true;
		}

		file.setFileName(fileName);
		text = "<b>" + QDir::toNativeSeparators(fileName) + "</b>";

		if (ui->filePreviewSpinBox->value() > 0) {
			text.append("<br />");

			if (file.open(QIODevice::ReadOnly)) {
				for (int i = 0; i < ui->filePreviewSpinBox->value(); i++) {
					char buf[1024];
					qint64 lineLength = file.readLine(buf, sizeof(buf));

					if (lineLength != -1) {
						text.append(buf);
					}
				}

				file.close();

				if (text.endsWith('\n')) {
					text.remove(text.size() - 1, 1);
				}
			}
		}

		if (!text.isEmpty()) {
			if (text.length() < fileName.size()) {
				key = "<p style='white-space:normal'>";
			} else {
				key = "<p style='white-space:pre'>";
			}

			QToolTip::showText(helpEvent->globalPos(), key + text, treeView, QRect());
		} else {
			QToolTip::hideText();
			event->ignore();
		}

		return true;
	}

	return QWidget::eventFilter(obj, event);
}
