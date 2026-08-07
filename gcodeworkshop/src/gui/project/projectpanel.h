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

#ifndef GUI_PROJECTPANEL_H
#define GUI_PROJECTPANEL_H

#include <QObject>  // for slots, Q_OBJECT, signals
#include <QString>  // for QString
#include <QWidget>  // for QWidget

class QModelIndex;
class QSettings;
class QStandardItem;
class QStandardItemModel;

namespace Ui {
class ProjectPanel;
}


namespace GUI {
class ProjectPanel : public QWidget
{
	Q_OBJECT

public:
	explicit ProjectPanel(QWidget* parent = nullptr);

public slots:
	bool maybeSave();

	void loadTranslations();
	void loadIcons();

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg) const;

signals:
	void fileClicked(const QString& fileName, const QString& mime);

protected slots:
	void newProject();
	void openProject();
	void saveProject();
	void saveProjectAs();
	void addItem();
	void removeItem();
	void treeViewDoubleClicked(const QModelIndex& index);

private:
	void load(const QString& projectName);
	QString selectName();

	QStandardItem* m_projectItem;
	bool m_modified;
	QString m_filePath;
	QStandardItemModel* m_model;
	Ui::ProjectPanel* ui;
};
} // namespace GUI

#endif // GUI_PROJECTPANEL_H
