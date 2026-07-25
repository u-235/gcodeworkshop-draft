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

#ifndef FINDTOOLBARACTIONS_H
#define FINDTOOLBARACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // IWYU pragma: keep

class QAction;

#include <gui/actions/actionkit.h> // for ActionKit

class GCodeWorkShop;

namespace GUI {
class FindToolBarActions : public Actions::ActionKit
{
	Q_OBJECT

public:
	explicit FindToolBarActions(GCodeWorkShop* parent);
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
} // namespace GUI

#endif // FINDTOOLBARACTIONS_H
