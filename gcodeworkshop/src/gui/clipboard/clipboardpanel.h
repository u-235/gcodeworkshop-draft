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

#ifndef GUI_CLIPBOARDPANEL_H
#define GUI_CLIPBOARDPANEL_H

#include <QObject>      // for slots, Q_OBJECT, signals
#include <QString>      // for QString
#include <QWidget>      // for QWidget

class QSettings;
class QStandardItemModel;

namespace Ui {
class ClipboardPanel;
}


namespace GUI {
class ClipboardPanel : public QWidget
{
	Q_OBJECT

public:
	explicit ClipboardPanel(QWidget* parent = nullptr);
	~ClipboardPanel();

signals:
	void hitText(const QString& text);

public slots:
	void loadTranslations();
	void loadIcons();

	void addText(const QString& text);

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg);

	void load();
	void save();

protected:
	void addItem(const QString& text, const QString& title);
	QString textByIndex(const QModelIndex& id);

protected slots:
	void removeSelectedItems();
	void treeViewDoubleClick(const QModelIndex& id);
	void treeViewContextMenu(const QPoint& point);

private:
	QStandardItemModel* m_model;
	int m_maxItem;
	int m_maxTextSize;
	bool m_modified;
	Ui::ClipboardPanel* ui;
};
} // namespace GUI

#endif // GUI_FILEBROWSERPANEL_H
