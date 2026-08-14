/*
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
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

#include <QLatin1String>    // for QLatin1String
#include <QSettings>        // for QSettings
#include <QString>          // IWYU pragma: keep
#include <QVariant>         // for QVariant
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	#include <QList>
	using QStringList = QList<QString>;
#endif

#include "cleanupoptions.h"


#define CFG_KEY_COMMENTS    (QLatin1String{"Comments"})
#define CFG_KEY_EXPRESSIONS (QLatin1String{"Expressions"})
#define CFG_KEY_SELECTED    (QLatin1String{"Selected"})


CleanUpOptions::CleanUpOptions() :
	expressions{},
	comments{},
	selected{}
{
}

void CleanUpOptions::load(QSettings* settings)
{
	comments = settings->value(CFG_KEY_COMMENTS, comments).toStringList();
	expressions = settings->value(CFG_KEY_EXPRESSIONS, expressions).toStringList();
	selected = settings->value(CFG_KEY_SELECTED, selected).toStringList();
}

void CleanUpOptions::save(QSettings* settings)
{
	if (expressions.isEmpty()) {
		settings->remove(CFG_KEY_COMMENTS);
		settings->remove(CFG_KEY_EXPRESSIONS);
		settings->remove(CFG_KEY_SELECTED);
	} else {
		settings->setValue(CFG_KEY_COMMENTS, comments);
		settings->setValue(CFG_KEY_EXPRESSIONS, expressions);
		settings->setValue(CFG_KEY_SELECTED, selected);
	}
}
