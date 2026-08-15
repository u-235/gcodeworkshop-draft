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

#include <QAbstractItemView>    // for QAbstractItemView
#include <QDir>                 // for QDir
#include <QFileInfo>            // for QFileInfo
#include <QHeaderView>          // for QHeaderView
#include <QIcon>                // for QIcon
#include <QList>                // for QList
#include <QSize>                // for QSize
#include <QStringList>          // for QStringList
#include <QTableWidgetItem>     // for QTableWidgetItem
#include <QVariant>             // for QVariant
#include <Qt>                   // for ContextMenuPolicy, PenStyle
#include <QtGlobal>             // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic

class QWidget;

#include <document.h>           // for Document
#include <documentmanager.h>    // for DocumentManager

#include "filetablepanel.h"


#define COLUMN_INFO   0
#define COLUMN_FILE   1
#define COLUMN_CLOSE  2


GUI::FileTablePanel::FileTablePanel(DocumentManager* docman, QWidget* parent) :
	QTableWidget{parent},
	m_documentManager(docman)
{
	setColumnCount(3);
	setHorizontalHeaderItem(COLUMN_INFO, new QTableWidgetItem());
	setHorizontalHeaderItem(COLUMN_FILE, new QTableWidgetItem());
	setHorizontalHeaderItem(COLUMN_CLOSE, new QTableWidgetItem());
	setObjectName(QString::fromUtf8("openFileTableWidget"));
	setMouseTracking(false);
	setContextMenuPolicy(Qt::NoContextMenu);
	setAutoFillBackground(true);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setProperty("showDropIndicator", QVariant(false));
	setDragDropOverwriteMode(false);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSortingEnabled(false);
	setIconSize(QSize(12, 12));
	setShowGrid(false);
	setGridStyle(Qt::NoPen);
	setWordWrap(false);
	setCornerButtonEnabled(false);
	verticalHeader()->setVisible(false);
	GUI::FileTablePanel::loadTranslations();

	connect(this, &QTableWidget::cellClicked, this,  &GUI::FileTablePanel::tableWidgetClicked);
}

GUI::FileTablePanel::~FileTablePanel()
{
}

void GUI::FileTablePanel::loadTranslations()
{
	setToolTip(tr("Open files"));
	horizontalHeaderItem(COLUMN_INFO)->setText(tr("Info"));
	horizontalHeaderItem(COLUMN_FILE)->setText(tr("File Name"));
	horizontalHeaderItem(COLUMN_CLOSE)->setText(tr("x"));


	for (int i = 0; i < rowCount(); i++) {
		item(i, COLUMN_CLOSE)->setToolTip(tr("Close"));
	}
}

void GUI::FileTablePanel::loadIcons()
{
	for (int i = 0; i < rowCount(); i++) {
		item(i, COLUMN_CLOSE)->setIcon(QIcon(":/images/fileclose_small.png"));
	}
}

void GUI::FileTablePanel::updateFileList()
{
	QFileInfo fileInfo;
	const QList<Document*>& docList = m_documentManager->documentList();

	setUpdatesEnabled(false);

	clearContents();
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	setRowCount(docList.size());

	for (int i = 0; i < docList.size(); ++i) {
		Document* doc = docList.at(i);

		fileInfo.setFile(doc->filePath());

		QTableWidgetItem* fileItem = new QTableWidgetItem(fileInfo.fileName() + (doc->isModified() ? "*" : ""));

		if (fileInfo.canonicalFilePath().isEmpty()) {
			fileItem->setToolTip(doc->filePath());
		} else {
			fileItem->setToolTip(QDir::toNativeSeparators(fileInfo.canonicalFilePath()));
		}

		setItem(i, COLUMN_FILE, fileItem);

		QTableWidgetItem* infoItem = new QTableWidgetItem(doc->brief());
		infoItem->setToolTip(doc->brief() + " --> " + QDir::toNativeSeparators(
		                         fileInfo.canonicalFilePath()));
		setItem(i, COLUMN_INFO, infoItem);

		QTableWidgetItem* closeItem = new QTableWidgetItem(QIcon(":/images/fileclose_small.png"), "",
		        QTableWidgetItem::UserType);
		closeItem->setToolTip(tr("Close"));
		setItem(i, COLUMN_CLOSE, closeItem);

		if (doc == m_documentManager->activeDocument()) {
			selectRow(i);
		}
	}

	resizeRowsToContents();
	horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

	setUpdatesEnabled(true);
}

void GUI::FileTablePanel::tableWidgetClicked(int x, int y)
{
	const QString& filePath = item(x, 1)->toolTip();
	Document* doc = m_documentManager->findDocumentByFilePath(filePath);

	if (doc) {
		if (y == COLUMN_CLOSE) {
			doc->close();
		} else {
			m_documentManager->setActiveDocument(doc);
		}
	}
}
