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

#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHelpEvent>
#include <QToolTip>
#include <QUrl>

#include <gcodeworkshop.h>

#include "filebrowser.h"
#include "ui_filebrowser.h" // for Ui::FileBrowserForm


Ui::FileBrowser::FileBrowser(QWidget* parent) :
	QWidget{parent},
	m_dirModel(new QFileSystemModel()),
	m_filters(),
	m_form(new FileBrowserForm())
{
	m_form->setupUi(this);

	m_dirModel->setResolveSymlinks(true);
	m_dirModel->setNameFilters(m_filters);
	m_dirModel->setNameFilterDisables(false);
	m_dirModel->setFilter(QDir::Files | QDir::AllDirs | QDir::Drives | QDir::NoDot);
	m_form->fileTreeView->setModel(m_dirModel);
	setRootPath(QDir::currentPath());

	connect(m_form->fileTreeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fileTreeViewDoubleClicked(QModelIndex)));
}

Ui::FileBrowser::~FileBrowser()
{
	delete m_form;
}

void Ui::FileBrowser::setRootPath(const QString& path)
{
	m_dirModel->setRootPath(path);
	m_form->fileTreeView->setRootIndex(m_dirModel->index(path));
	m_form->fileTreeView->setToolTip(path);
	m_form->fileTreeView->setSortingEnabled(true);
	m_form->fileTreeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	m_form->fileTreeView->resizeColumnToContents(0);
	m_form->fileTreeView->resizeColumnToContents(1);
	m_form->fileTreeView->setColumnHidden(2, true);
	m_form->fileTreeView->resizeColumnToContents(3);
}

bool Ui::FileBrowser::event(QEvent* event)
{
	QString key, text;
	QModelIndex index;
	QFile file;
	QString fileName;

	if ((event->type() == QEvent::ToolTip)) {
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

		QPoint pos = m_form->fileTreeView->viewport()->mapFromGlobal(helpEvent->globalPos());

		if ((pos.y() >= m_form->fileTreeView->viewport()->height()) ||
		        (pos.x() >= m_form->fileTreeView->viewport()->width())) {
			return true;
		}

		index = m_form->fileTreeView->indexAt(pos);

		if (!index.isValid()) {
			return true;
		}

		fileName = m_dirModel->filePath(index);
		file.setFileName(fileName);
		text = "<b>" + QDir::toNativeSeparators(fileName) + "</b>";

		if (m_form->filePreviewSpinBox->value() > 0) {
			text.append("<br />");

			if (file.open(QIODevice::ReadOnly)) {
				for (int i = 0; i < m_form->filePreviewSpinBox->value(); i++) {
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

			QToolTip::showText(helpEvent->globalPos(), key + text, this, QRect());
		} else {
			QToolTip::hideText();
			event->ignore();
		}

		return true;
	}

	return QWidget::event(event);
}

void Ui::FileBrowser::setNameFilters(const QStringList& filters)
{
	m_filters = filters;
}

void Ui::FileBrowser::currentDirChanged(const QString& path)
{
	if (path.isEmpty()) {
		return;
	}

	if (m_form->currentPathCheckBox->isChecked()) {
		setRootPath(path);
	}
}

void Ui::FileBrowser::fileTreeViewDoubleClicked(const QModelIndex& index)
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
			emit fileClicked(info.absoluteFilePath());
		}
	}
}
