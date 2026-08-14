/*
 *  Copyright (C) 2023-2025 Nick Egorrov, nicegorov@yandex.ru
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

#include "renumberoptions.h"


#define CFG_KEY_APPLY_WIDTH     (QLatin1String{"ApplyWidth"})
#define CFG_KEY_FROM            (QLatin1String{"From"})
#define CFG_KEY_INC             (QLatin1String{"Inc"})
#define CFG_KEY_KEEP_EXISTING   (QLatin1String{"KeepExisting"})
#define CFG_KEY_MODE            (QLatin1String{"Mode"})
#define CFG_KEY_RENUM_EMPTY     (QLatin1String{"RenumEmpty"})
#define CFG_KEY_RENUM_MARKED    (QLatin1String{"RenumMarked"})
#define CFG_KEY_RENUM_WITH_COMM (QLatin1String{"RenumWithComm"})
#define CFG_KEY_START           (QLatin1String{"Start"})
#define CFG_KEY_TO              (QLatin1String{"To"})
#define CFG_KEY_WIDTH           (QLatin1String{"Width"})


RenumberOptions::RenumberOptions() :
	mode{RenumberAll},
	from{0},
	to{999999},
	startAt{10},
	inc{10},
	width{6},
	applyWidth{false},
	renumEmpty{false},
	renumComm{false},
	renumMarked{true},
	keepExisting{true}
{
}

void RenumberOptions::load(QSettings* settings)
{
	applyWidth = settings->value(CFG_KEY_APPLY_WIDTH, applyWidth).toBool();
	from = settings->value(CFG_KEY_FROM, from).toInt();
	inc = settings->value(CFG_KEY_INC, inc).toInt();
	mode = settings->value(CFG_KEY_MODE, mode).toInt();
	startAt = settings->value(CFG_KEY_START, startAt).toInt();
	to = settings->value(CFG_KEY_TO, to).toInt();
	renumComm = settings->value(CFG_KEY_RENUM_WITH_COMM, renumComm).toBool();
	renumEmpty = settings->value(CFG_KEY_RENUM_EMPTY, renumEmpty).toBool();
	renumMarked = settings->value(CFG_KEY_RENUM_MARKED, renumMarked).toBool();
	keepExisting = settings->value(CFG_KEY_KEEP_EXISTING, keepExisting).toBool();
	width = settings->value(CFG_KEY_WIDTH, width).toInt();
}

void RenumberOptions::save(QSettings* settings)
{
	settings->setValue(CFG_KEY_APPLY_WIDTH, applyWidth);
	settings->setValue(CFG_KEY_FROM, from);
	settings->setValue(CFG_KEY_INC, inc);
	settings->setValue(CFG_KEY_MODE, mode);
	settings->setValue(CFG_KEY_RENUM_WITH_COMM, renumComm);
	settings->setValue(CFG_KEY_RENUM_EMPTY, renumEmpty);
	settings->setValue(CFG_KEY_RENUM_MARKED, renumMarked);
	settings->setValue(CFG_KEY_KEEP_EXISTING, keepExisting);
	settings->setValue(CFG_KEY_START, startAt);
	settings->setValue(CFG_KEY_TO, to);
	settings->setValue(CFG_KEY_WIDTH, width);
}
