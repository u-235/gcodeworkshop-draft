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

#ifndef UI_FINDTOOLBAR_FINDTOOLBARACTIONS_H
#define UI_FINDTOOLBAR_FINDTOOLBARACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;

#include <ui/actions/actionkit.h>   // for ActionKit

class GCodeWorkShop;

namespace Ui {
class FindToolBarActions : public Actions::ActionKit
{
	Q_OBJECT

public:
	explicit FindToolBarActions(::GCodeWorkShop* parent);
	~FindToolBarActions();

public slots:
	void loadTranslations() override;
	void loadIcons() override;

public:
	QAction* findNext();
	QAction* findPrevious();
	QAction* replaceNext();
	QAction* replacePrevious();
	QAction* replaceAll();
	QAction* findClose();
};
} // namespace Ui

#endif // UI_FINDTOOLBARACTIONS_H
