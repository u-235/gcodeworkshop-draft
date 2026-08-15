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
#include <QHeaderView>          // for QHeaderView
#include <QIcon>                // for QIcon
#include <QSize>                // for QSize
#include <QTableWidgetItem>     // for QTableWidgetItem
#include <QVariant>             // for QVariant
#include <Qt>                   // for ContextMenuPolicy, PenStyle

class QWidget;

#include <document.h>           // for Document
#include <documentmanager.h>    // for DocumentManager

#include "filetablepanel.h"


#define COLUMN_INFO   0
#define COLUMN_FILE   1
#define COLUMN_CLOSE  2


GUI::FileTablePanel::FileTablePanel(DocumentManager* docman, QWidget* parent) :
	QTableWidget{parent},
	m_documentManager(docman),
	m_documentList()
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
	m_documentList = m_documentManager->documentList();

	setUpdatesEnabled(false);

	clearContents();
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	setRowCount(m_documentList.size());

	for (int i = 0; i < m_documentList.size(); ++i) {
		updateDocumentModifityById(i);
		updateDocumentInfoById(i);

		QTableWidgetItem* closeItem = new QTableWidgetItem(QIcon(":/images/fileclose_small.png"), "",
		        QTableWidgetItem::UserType);
		closeItem->setToolTip(tr("Close"));
		setItem(i, COLUMN_CLOSE, closeItem);
	}

	resizeRowsToContents();
	horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

	setUpdatesEnabled(true);
}

void GUI::FileTablePanel::updateDocumentInfo(Document* doc)
{
	updateDocumentInfoById(documentIndex(doc));
}

void GUI::FileTablePanel::updateDocumentModifity(Document* doc)
{
	updateDocumentModifityById(documentIndex(doc));
}

void GUI::FileTablePanel::updateActiveDocument(Document* doc)
{
	selectRow(documentIndex(doc));
}

void GUI::FileTablePanel::updateDocumentInfoById(int id)
{
	if (id < 0) {
		return;
	}

	Document* doc = m_documentList.at(id);
	QTableWidgetItem* infoItem = item(id, COLUMN_INFO);

	if (infoItem == nullptr) {
		infoItem = new QTableWidgetItem();
		setItem(id, COLUMN_INFO, infoItem);
	}

	infoItem->setText(doc->brief());
	infoItem->setToolTip(doc->brief() + " --> " + QDir::toNativeSeparators(doc->filePath()));
}

void GUI::FileTablePanel::updateDocumentModifityById(int id)
{
	if (id < 0) {
		return;
	}

	Document* doc = m_documentList.at(id);
	QTableWidgetItem* fileItem = item(id, COLUMN_FILE);

	if (fileItem == nullptr) {
		fileItem = new QTableWidgetItem();
		setItem(id, COLUMN_FILE, fileItem);
	}

	fileItem->setText(doc->fileName() + (doc->isModified() ? "*" : ""));
	fileItem->setToolTip(QDir::toNativeSeparators(doc->filePath()));
}

int GUI::FileTablePanel::documentIndex(Document* doc)
{
	for (int i = 0; i < m_documentList.size(); ++i) {
		if (doc == m_documentList.at(i)) {
			return i;
		}
	}

	return -1;
}

void GUI::FileTablePanel::tableWidgetClicked(int x, int y)
{
	if (!m_documentList.empty()) {
		Document* doc = m_documentList[x];

		if (y == COLUMN_CLOSE) {
			doc->close();
		} else {
			m_documentManager->setActiveDocument(doc);
		}
	}
}
