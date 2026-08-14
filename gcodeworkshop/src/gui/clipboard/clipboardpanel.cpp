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
#include <QFont>                // for QFont
#include <QIcon>                // for QIcon
#include <QItemSelectionModel>  // for QItemSelectionModel
#include <QList>                // for QList
#include <QModelIndex>          // for QModelIndex
#include <QModelIndexList>      // for QModelIndexList
#include <QSettings>            // for QSettings
#include <QStandardItemModel>   // for QStandardItem, QStandardItemModel
#include <QStringList>          // for QStringList
#include <QToolButton>          // for QToolButton
#include <QTreeView>            // for QTreeView
#include <QVariant>             // for QVariant
#include <Qt>                   // for ContextMenuPolicy, SortOrder
#include <QtGlobal>             // for Q_UNUSED

#include <utils/medium.h>       // for Medium

#include "clipboardpanel.h"
#include "ui_clipboardpanel.h"  // for Ui::ClipboardPanel


#define DEFAULT_ITEMS_LIMIT 5
#define DEFAULT_TEXT_LIMIT  2000

#define CFG_SECTION             "ClipboardPanel"
#define CFG_KEY_MAX_ITEM_COUNT  "MaxItemCount"
#define CFG_KEY_MAX_TEXT_SIZE   "MaxTextSize"

#define CBF_FILENAME   "/clipboard"
#define CBF_ITEMS      "ClipboardItems"
#define CBF_ITEM_TEXT  "ItemText"
#define CBF_ITEM_TITLE "Title"


GUI::ClipboardPanel::ClipboardPanel(QWidget* parent) :
	QWidget{parent},
	m_model(new QStandardItemModel()),
	m_maxItem(DEFAULT_ITEMS_LIMIT),
	m_maxTextSize(DEFAULT_TEXT_LIMIT),
	m_modified(false),
	ui(new Ui::ClipboardPanel())
{
	ui->setupUi(this);
	GUI::ClipboardPanel::loadIcons();
	ui->treeView->setModel(m_model);
	ui->treeView->setRootIsDecorated(false);
	ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->btn_remove, &QToolButton::clicked, this, &GUI::ClipboardPanel::removeSelectedItems);
	connect(ui->treeView, &QTreeView::doubleClicked, this, &GUI::ClipboardPanel::treeViewDoubleClick);
	connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &GUI::ClipboardPanel::treeViewContextMenu);
}

GUI::ClipboardPanel::~ClipboardPanel()
{
	delete ui;
}

void GUI::ClipboardPanel::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::ClipboardPanel::loadIcons()
{
	ui->btn_remove->setIcon(QIcon(":/images/list-remove.png"));
	QStandardItem* root = m_model->invisibleRootItem();

	for (int i = 0; i < root->rowCount(); i++) {
		root->child(i, 0)->setIcon(QIcon(":/images/editpaste.png"));
	}
}

void GUI::ClipboardPanel::addText(const QString& text)
{
	QString limitedText = text;
	limitedText.truncate(m_maxTextSize);

	if (limitedText.isEmpty()) {
		return;
	}

	QStandardItem* root = m_model->invisibleRootItem();

	// check that text is already in clipboard
	for (int i = 0; i < root->rowCount(); i++) {
		QStandardItem* item = root->child(i, 0);

		if (limitedText == item->child(0, 0)->text()) {
			root->removeRow(i);
			break;
		}
	}

	if (root->rowCount() >= m_maxItem) {
		QStandardItem* item = root->child(m_maxItem, 0);

		if (item) {
			if (item->text() == "") {
				root->removeRow(m_maxItem);
			}
		}
	}

	addItem(limitedText, "");
	m_modified = true;
	ui->treeView->expandAll();
}

void GUI::ClipboardPanel::loadSettings(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);
	m_maxItem = cfg->value(CFG_KEY_MAX_ITEM_COUNT, DEFAULT_ITEMS_LIMIT).toInt();
	m_maxTextSize = cfg->value(CFG_KEY_MAX_TEXT_SIZE, DEFAULT_TEXT_LIMIT).toInt();
	cfg->endGroup();

	if (!m_modified) {
		load();
	}
}

void GUI::ClipboardPanel::saveSettings(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);
	cfg->setValue(CFG_KEY_MAX_ITEM_COUNT, m_maxItem);
	cfg->setValue(CFG_KEY_MAX_TEXT_SIZE, m_maxTextSize);
	cfg->endGroup();

	save();
}

void GUI::ClipboardPanel::load()
{
	m_model->clear();
	m_model->setColumnCount(1);
	m_model->setHorizontalHeaderLabels(QStringList() << tr("Clipboard"));

	QSettings settings(Medium::instance().settingsDir() + CBF_FILENAME, QSettings::IniFormat);

	int max = settings.beginReadArray(CBF_ITEMS);

	for (int i = 0; i < max; ++i) {
		settings.setArrayIndex(i);
		const QString& title  = settings.value(CBF_ITEM_TITLE, "").toString();
		const QString& text = settings.value(CBF_ITEM_TEXT, "").toString();
		addItem(text, title);
	}

	settings.endArray();

	ui->treeView->expandAll();
	m_modified = false;
}

void GUI::ClipboardPanel::save()
{
	QSettings settings(Medium::instance().settingsDir() + CBF_FILENAME, QSettings::IniFormat);

	settings.remove(CBF_ITEMS);
	settings.beginWriteArray(CBF_ITEMS);

	QStandardItem* parentItem = m_model->invisibleRootItem();

	for (int i = 0; i < parentItem->rowCount(); i++) {
		QStandardItem* item = parentItem->child(i, 0);

		settings.setArrayIndex(i);
		settings.setValue(CBF_ITEM_TITLE, item->text());
		settings.setValue(CBF_ITEM_TEXT, item->child(0, 0)->text());
	}

	settings.endArray();
	m_modified = false;
}

void GUI::ClipboardPanel::addItem(const QString& text, const QString& title)
{
	QStandardItem* root = m_model->invisibleRootItem();
	QStandardItem* titleItem = new QStandardItem(QIcon(":/images/editpaste.png"), title);
	titleItem->setEditable(true);
	QFont font = titleItem->font();
	font.setBold(true);
	font.setPointSize(font.pointSize() + 1);
	titleItem->setFont(font);
	root->insertRow(0, titleItem);

	QStandardItem* textItem = new QStandardItem(text);
	textItem->setEditable(false);
	font = textItem->font();
	font.setFixedPitch(true);
	textItem->setFont(font);
	titleItem->appendRow(textItem);
}

QString GUI::ClipboardPanel::textByIndex(const QModelIndex& id)
{
	QStandardItem* item = m_model->itemFromIndex(id);

	if (item == nullptr) {
		return QString();
	}

	if (item->hasChildren()) {
		item = item->child(0, 0);
	}

	return item->text();
}

void GUI::ClipboardPanel::removeSelectedItems()
{
	const QModelIndexList& list = ui->treeView->selectionModel()->selectedIndexes();

	for (const QModelIndex& it : list) {
		QStandardItem* item = m_model->itemFromIndex(it);

		if (!item->hasChildren()) {
			item = item->parent();
		}

		m_model->removeRow(item->row(), m_model->invisibleRootItem()->index());
	}
}

void GUI::ClipboardPanel::treeViewDoubleClick(const QModelIndex& id)
{
	emit hitText(textByIndex(id));
}

void GUI::ClipboardPanel::treeViewContextMenu(const QPoint& point)
{
	QModelIndex id = ui->treeView->indexAt(point);
	emit hitText(textByIndex(id));
}
