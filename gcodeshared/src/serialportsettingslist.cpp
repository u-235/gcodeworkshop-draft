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

#include <stdexcept>    // for invalid_argument

#include <QCoreApplication> // for QCoreApplication
#include <QSettings>        // for QSettings
#include <QVariant>         // for QVariant
#include <QtDebug>          // for QDebug
#include <QtGlobal>         // for qWarning

#include <serialportsettings.h>         // for SerialPortSettings
#include <serialportsettingslist.h>

#define tr(s) QCoreApplication::translate("SerialPortSettingsList", s)

#define CFG_SECTION             "SerialPortConfigs"
#define CFG_KEY_ACTIVE_PROFILE  "CurrentSerialPortSettings"
#define CFG_KEY_PROFILE_NAMES   "SettingsList"


SerialPortSettingsList::SerialPortSettingsList() :
	m_profiles({{tr("Default"), SerialPortSettings()}}),
	m_active(tr("Default"))
{
}

SerialPortSettingsList::~SerialPortSettingsList()
{
}

QStringList SerialPortSettingsList::names()
{
	return m_profiles.keys();
}

bool SerialPortSettingsList::contains(const QString& name) const
{
	return m_profiles.contains(name);
}

QString SerialPortSettingsList::activeName() const
{
	return m_active;
}

bool SerialPortSettingsList::setActiveName(const QString& name)
{
	if (!contains(name)) {
		return false;
	}

	m_active = name;
	return true;
}

SerialPortSettings SerialPortSettingsList::profile(const QString& name) const
{
	if (!contains(name)) {
		throw std::invalid_argument("SerialPortSettingsManager.profile: inalid name");
	}

	return m_profiles.value(name);
}

void SerialPortSettingsList::setProfile(const QString& name, const SerialPortSettings& profile)
{
	m_profiles.insert(name, profile);
}

void SerialPortSettingsList::removeProfile(const QString& name)
{
	if (!contains(name)) {
		return;
	}

	m_profiles.remove(name);
}

void SerialPortSettingsList::load(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);

	const QStringList& childs = cfg->childGroups();

	if (childs.isEmpty()) {
		qWarning() << "SerialPortSettingsManager.load: childGroups is empty!";
	} else {
		m_profiles.clear();

		for (const QString& name : childs) {
			SerialPortSettings ps{};
			ps.configName = name;
			ps.load(cfg);
			m_profiles.insert(name, ps);
		}

		const QString& active = cfg->value(CFG_KEY_ACTIVE_PROFILE, tr("Default")).toString();

		if (m_profiles.contains(active)) {
			m_active = active;
		} else {
			m_active = m_profiles.firstKey();
		}
	}

	cfg->endGroup();
}

void SerialPortSettingsList::save(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);
	cfg->remove("");
	cfg->setValue(CFG_KEY_ACTIVE_PROFILE, m_active);
	cfg->setValue(CFG_KEY_PROFILE_NAMES, names());

	for (auto pro : m_profiles) {
		pro.save(cfg);
	}

	cfg->endGroup();
}
