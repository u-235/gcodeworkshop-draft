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

#ifndef UI_FILEBROWSER_FILEBROWSER_H
#define UI_FILEBROWSER_FILEBROWSER_H

#include <QWidget>

class QFileSystemModel;

namespace Ui {
class FileBrowserForm;

class FileBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit FileBrowser(QWidget* parent = nullptr);
	~FileBrowser();

	void setRootPath(const QString& path);

signals:
	void fileClicked(const QString& fileName);

protected:
	bool event(QEvent* event) override;

public slots:
	void setNameFilters(const QStringList& filters);
	void currentDirChanged(const QString& path);

protected slots:
	void fileTreeViewDoubleClicked(const QModelIndex& index);

private:
	QFileSystemModel* m_dirModel;
	QStringList m_filters;
	FileBrowserForm* m_form;
};
} // namespace Ui

#endif // UI_FILEBROWSER_FILEBROWSER_H
