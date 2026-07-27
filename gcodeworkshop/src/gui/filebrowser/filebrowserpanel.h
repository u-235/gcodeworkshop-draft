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

#ifndef GUI_FILEBROWSERPANEL_H
#define GUI_FILEBROWSERPANEL_H

#include <QObject>      // for slots, Q_OBJECT, signals
#include <QString>      // for QString
#include <QStringList>  // for QStringList
#include <QWidget>      // for QWidget

class QFileSystemModel;
class QModelIndex;
class QSettings;

namespace Ui {
class FileBrowserPanel;
}


namespace GUI {
class FileBrowserPanel : public QWidget
{
	Q_OBJECT

public:
	explicit FileBrowserPanel(QWidget* parent = nullptr);
	~FileBrowserPanel();

	void setRootPath(const QString& path);

signals:
	void fileClicked(const QString& fileName, const QString& mime);

public slots:
	void setNameFilters(const QStringList& filters);
	void currentDirChanged(const QString& path);

	void loadTranslations();

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg) const;

protected slots:
	void fileTreeViewDoubleClicked(const QModelIndex& index);
	void followCheckBoxToggled(bool checked);
	void filterCheckBoxToggled(bool checked);

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QFileSystemModel* m_dirModel;
	QStringList m_filters;
	QString m_currentDir;
	Ui::FileBrowserPanel* ui;
};
} // namespace GUI

#endif // GUI_FILEBROWSERPANEL_H
