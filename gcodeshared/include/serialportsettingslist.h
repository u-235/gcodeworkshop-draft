/*
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

#ifndef SERIALPORTSETTINGSLIST_H
#define SERIALPORTSETTINGSLIST_H

#include <QMap>         // for QMap
#include <QString>      // for QString
#include <QStringList>  // for QStringList

class QSettings;

class SerialPortSettings;


struct SerialPortSettingsList {
	SerialPortSettingsList();
	~SerialPortSettingsList();

	QStringList names();
	bool contains(const QString& name) const;

	QString activeName() const;
	bool setActiveName(const QString& name);

	SerialPortSettings profile(const QString& name) const;
	void setProfile(const QString& name, const SerialPortSettings& profile);
	void removeProfile(const QString& name);

	void load(QSettings* cfg);
	void save(QSettings* cfg);

protected:
	QMap<QString, SerialPortSettings> m_profiles;
	QString m_active;
};

#endif // SERIALPORTSETTINGSLIST_H
