/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
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

#ifndef GUI_SERIALTOOLBAR_H
#define GUI_SERIALTOOLBAR_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QPointer> // for QPointer
#include <QString>  // for QString
#include <QToolBar>
#include <QWidget>

class QComboBox;
class QSettings;

class CapsLockEventFilter;
class GCoderDocument;
class GCodeWorkShop;

namespace GUI {
class SerialToolBarActions;

namespace Actions {
class ActionKit;
}


class SerialToolBar : public QToolBar
{
	Q_OBJECT

public:
	explicit SerialToolBar(GCodeWorkShop* app, QWidget* parent);
	~SerialToolBar();

public:
	bool selectProfile(const QString& name);

public slots:
	GUI::Actions::ActionKit* actions() const;

	void loadTranslations();
	void loadIcons();

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg) const;

	void loadSerialConfignames();

protected slots:
	void configure();
	void testConfig();
	void send();
	void receive();
	void attachToDir();
	void deAttachToDir();

protected:
	GCodeWorkShop* m_app;
	SerialToolBarActions* m_actions;
	QComboBox* m_configBox;
};
} // namespace GUI

#endif // GUI_SERIALTOOLBAR_H
