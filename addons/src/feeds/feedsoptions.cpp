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
#include <QVariant>         // for QVariant

#include "feedsoptions.h"


#define CFG_KEY_DIAM_VALUE  (QLatin1String{"Diam"})
#define CFG_KEY_DIAM_IN     (QLatin1String{"DiamIn"})
#define CFG_KEY_FEED_VALUE  (QLatin1String{"Feed"})
#define CFG_KEY_FEED_IN     (QLatin1String{"FeedIn"})
#define CFG_KEY_FZ_VALUE    (QLatin1String{"Fz"})
#define CFG_KEY_FZ_IN       (QLatin1String{"FzIn"})
#define CFG_KEY_SPEED_VALUE (QLatin1String{"Speed"})
#define CFG_KEY_SPEED_IN    (QLatin1String{"SpeedIn"})
#define CFG_KEY_USE_INCH    (QLatin1String{"UseInch"})
#define CFG_KEY_VC_VALUE    (QLatin1String{"Vc"})
#define CFG_KEY_VC_IN       (QLatin1String{"VcIn"})
#define CFG_KEY_Z_VALUE     (QLatin1String{"z"})
#define CFG_KEY_Z_IN        (QLatin1String{"zIn"})


FeedsOptions::FeedsOptions() :
	useInch{false},
	Vc{true, 100.0},
	z{true, 1.0},
	Fz{true, 0.3},
	diam{true, 32.0},
	speed{false, 0.0},
	feed{false, 0.0}
{
}

void FeedsOptions::load(QSettings* settings)
{
	diam.value = settings->value(CFG_KEY_DIAM_VALUE, diam.value).toDouble();
	diam.in = settings->value(CFG_KEY_DIAM_IN, diam.in).toBool();
	feed.value = settings->value(CFG_KEY_FEED_VALUE, feed.value).toDouble();
	feed.in = settings->value(CFG_KEY_FEED_IN, feed.in).toBool();
	Fz.value = settings->value(CFG_KEY_FZ_VALUE, Fz.value).toDouble();
	Fz.in = settings->value(CFG_KEY_FZ_IN, Fz.in).toBool();
	speed.value = settings->value(CFG_KEY_SPEED_VALUE, speed.value).toDouble();
	speed.in = settings->value(CFG_KEY_SPEED_IN, speed.in).toBool();
	Vc.value = settings->value(CFG_KEY_VC_VALUE, Vc.value).toDouble();
	Vc.in = settings->value(CFG_KEY_VC_IN, Vc.in).toBool();
	useInch = settings->value(CFG_KEY_USE_INCH, useInch).toBool();
	z.value = settings->value(CFG_KEY_Z_VALUE, z.value).toDouble();
	z.in = settings->value(CFG_KEY_Z_IN, z.in).toBool();
}

void FeedsOptions::save(QSettings* settings)
{
	settings->setValue(CFG_KEY_DIAM_VALUE, diam.value);
	settings->setValue(CFG_KEY_DIAM_IN, diam.in);
	settings->setValue(CFG_KEY_FEED_VALUE, feed.value);
	settings->setValue(CFG_KEY_FEED_IN, feed.in);
	settings->setValue(CFG_KEY_FZ_VALUE, Fz.value);
	settings->setValue(CFG_KEY_FZ_IN, Fz.in);
	settings->setValue(CFG_KEY_SPEED_VALUE, speed.value);
	settings->setValue(CFG_KEY_SPEED_IN, speed.in);
	settings->setValue(CFG_KEY_VC_VALUE, Vc.value);
	settings->setValue(CFG_KEY_VC_IN, Vc.in);
	settings->setValue(CFG_KEY_USE_INCH, useInch);
	settings->setValue(CFG_KEY_Z_VALUE, z.value);
	settings->setValue(CFG_KEY_Z_IN, z.in);
}
