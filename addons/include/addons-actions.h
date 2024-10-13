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

#ifndef ADDONS_ACTIONS_H
#define ADDONS_ACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;

#include <gui/actions/actionkit.h> // for ActionKit

class GCodeWorkShop;


namespace Addons {
class Actions : public GUI::Actions::ActionKit
{
	Q_OBJECT

public:
	explicit Actions(GCodeWorkShop* parent);
	~Actions();

public slots:
	void loadTranslations() override;
	void loadIcons() override;

public:
	QAction* bhc();
	QAction* blockSkipDecrement();
	QAction* blockSkipIncrement();
	QAction* blockSkipRemove();
	QAction* chamfer();
	QAction* cleanUp();
	QAction* paraComment();
	QAction* semiComment();
	QAction* compileMacro();
	QAction* dot();
	QAction* insertEmptyLines();
	QAction* removeEmptyLines();
	QAction* feeds();
	QAction* i2m();
	QAction* i2mProg();
	QAction* renumber();
	QAction* insertSpaces();
	QAction* removeSpaces();
	QAction* splitProgramms();
	QAction* swapAxes();
	QAction* triangle();

protected:
	GCodeWorkShop* m_app;

protected slots:
	void doBhc();
	void doBlockSkipDecrement();
	void doBlockSkipIncrement();
	void doBlockSkipRemove();
	void doChamfer();
	void doCleanUp();
	void doParaComment();
	void doSemiComment();
	void doCompileMacro();
	void doDot();
	void doInsertEmptyLines();
	void doRemoveEmptyLines();
	void doFeeds();
	void doI2M();
	void doI2MProg();
	void doRenumber();
	void doInsertSpaces();
	void doRemoveSpaces();
	void doSplitProgramms();
	void doSwapAxes();
	void doTriangle();

protected:
	void doBlockSkip(bool remove, bool inc);
};
} // namespace Addons

#endif // ADDONS_ACTIONS_H
