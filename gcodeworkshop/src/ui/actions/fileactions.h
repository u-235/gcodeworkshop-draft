/*
 *  Copyright (C) 2024 Nick Egorrov, nicegorov@yandex.ru
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

#ifndef UI_ACTIONS_FILEACTIONS_H
#define UI_ACTIONS_FILEACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;

#include <ui/actions/actionkit.h>   // for ActionKit

class GCodeWorkShop;


namespace Ui {
namespace Actions {
class FileActions : public ActionKit
{
	Q_OBJECT

public:
	explicit FileActions(::GCodeWorkShop* app);
	~FileActions();

public slots:
	void loadTranslations() override;
	void loadIcons() override;

public:
	QAction* close();
	QAction* closeAll();
	QAction* exit();
	QAction* findFiles();
	QAction* makeNew();
	QAction* open();
	QAction* openExample();
	QAction* print();
	QAction* printPreview();
	QAction* save();
	QAction* saveAll();
	QAction* saveAs();
	QAction* sessionManager();
};
} // namespace Actions
} // namespace Ui

#endif // UI_ACTIONS_FILEACTIONS_H
