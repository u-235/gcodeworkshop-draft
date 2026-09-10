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

#include <QKeySequence> // for QKeySequence
#include <QSettings>    // for QSettings
#include <QString>      // for QString
#include <QStringList>  // for QStringList
#include <QVariant>     // for QVariant

#include <utils/keysequencemap.h>   // IWYU pragma: associated

#define CFG_SECTION  "Shortcuts"


KeySequenceMap::KeySequenceMap() : QMap()
{
}

KeySequenceMap::~KeySequenceMap()
{
}

void KeySequenceMap::load(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);
	const QStringList& keyList = cfg->childKeys();

	for (const QString& key : keyList) {
		insert(key, QKeySequence(cfg->value(key).toString()));
	}

	cfg->endGroup();
}

void KeySequenceMap::save(QSettings* cfg) const
{
	cfg->beginGroup(CFG_SECTION);
	cfg->remove("");
	QMap<QString, QKeySequence>::const_iterator e = end();

	for (QMap<QString, QKeySequence>::const_iterator i = begin(); i != e; ++i) {
		if (!i.value().isEmpty()) {
			cfg->setValue(i.key(), i.value().toString());
		}
	}

	cfg->endGroup();
}
