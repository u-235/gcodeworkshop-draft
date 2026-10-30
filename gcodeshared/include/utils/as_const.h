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

#ifndef UTILS_AS_CONST_H
#define UTILS_AS_CONST_H

#if __cplusplus >= 201703L
	#define asConst(s) (std::as_const(s))
#else
	#include <QtGlobal>
	#if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
		#define asConst(s) (s)
	#else
		#define asConst(s) (qAsConst(s))
	#endif
#endif

#endif // UTILS_AS_CONST_H
