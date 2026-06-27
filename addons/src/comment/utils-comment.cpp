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

#include <QChar>    // for QChar
#include <QString>  // for QString

#include "utils-comment.h"


bool Utils::autoComments(QString& text, int mode, const std::function<bool(int)>& interrupt)
{
	bool changed = false;
	bool remove = false;
	QString result;
	int substr_start = 0;
	int substr_end = 0;

	switch (mode) {
	case ParenthesisComments:
		if (text.startsWith(QChar{'('})) {
			remove = true;
		}

		break;

	case SemicolonComments:
		if (text.startsWith(QChar{';'})) {
			remove = true;
		}

		break;

	default:
		;
	}

	while (substr_end >= 0) {
		substr_end = text.indexOf(QChar{'\n'}, substr_start);
		QString line = text.mid(substr_start, substr_end - substr_start + 1);
		substr_start = substr_end + 1;
		bool localChange = false;

		if (interrupt(substr_start)) {
			return false;
		}

		switch (mode) {
		case ParenthesisComments:
			localChange = parenthesisComments(line, remove);
			break;

		case SemicolonComments:
			localChange = semicolonComments(line, remove);
			break;

		default:
			;
		}

		if (localChange) {
			changed = true;
		}

		result += line;
	}

	text = result;
	return changed;
}

bool Utils::parenthesisComments(QString& line, bool remove)
{
	int oldSize = line.size();

	if (remove && line.startsWith(QChar{'('})) {
		line.remove(0, 1);
		int pli = line.lastIndexOf(QChar{')'});

		if (pli >= 0) {
			line.remove(pli, 1);
		}
	}

	if (!remove) {
		line.prepend("(");
		int eol = line.indexOf(QChar{'\n'});

		if (eol < 0) {
			line.append(QChar{')'});
		} else {
			line.insert(eol, QChar{')'});
		}
	}

	return oldSize != line.size();
}

bool Utils::semicolonComments(QString& line, bool remove)
{
	int oldSize = line.size();

	if (remove && line.startsWith(QChar{';'})) {
		line.remove(0, 1);
	} else {
		line.prepend(QChar{';'});
	}

	return oldSize != line.size();
}
