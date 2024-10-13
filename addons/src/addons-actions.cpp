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

#include <functional>   // for function

#include <QAction>          // for QAction
#include <QApplication>     // for QApplication
#include <QIcon>            // for QIcon
#include <QList>            // for QList<>::iterator
#include <QLatin1String>    // for QLatin1String
#include <QMainWindow>      // for QMainWindow
#include <QMessageBox>      // for QMessageBox
#include <QStringList>      // for QStringList
#include <Qt>               // for BusyCursor

#include <gui/actions/actionkit.h>      // for ActionKit
#include <addons-actions.h>             // IWYU pragma: associated
#include <document.h>                   // for Document
#include <gcoderdocument.h>             // for GCoderDocument
#include <gcodeworkshop.h>              // for GCodeWorkShop
#include <gui/actions/knownactions.h>   // for KnownActions
#include <ui/longjobhelper.h>           // for LongJobHelper, LongJobHelper::CANCEL
#include <utils/medium.h>               // for Medium
#include <utils/splitfile.h>            // for splitFile

#include "addons-context.h" // for Context, Context::ALL, Context::SELECTED, Context::SELECTED_BLOCKS

#include "bhc/addons-bhc.h"
#include "blockskip/utils-blockskip.h"
#include "chamfer/addons-chamfer.h"
#include "cleanup/addons-cleanup.h"
#include "comment/utils-comment.h"
#include "compilemacro/utils-compilemacro.h"
#include "dot/addons-dot.h"
#include "emptylines/utils-emptylines.h"
#include "feeds/addons-feeds.h"
#include "i2m/addons-i2m.h"
#include "i2mprog/addons-i2mprog.h"
#include "renumber/addons-renumber.h"
#include "spaces/utils-spaces.h"
#include "swapaxes/addons-swapaxes.h"
#include "triangle/addons-triangle.h"


using ActionId = GUI::Actions::KnownActions::Addons;


Addons::Actions::Actions(GCodeWorkShop* parent) :
	GUI::Actions::ActionKit(parent),
	m_app(parent)
{
	connect(makeAction(ActionId::BHC), SIGNAL(triggered()), this, SLOT(doBhc()));
	connect(makeAction(ActionId::BLOCK_SKIP_DECREMENT), SIGNAL(triggered()), this, SLOT(doBlockSkipDecrement()));
	connect(makeAction(ActionId::BLOCK_SKIP_INCREMENT), SIGNAL(triggered()), this, SLOT(doBlockSkipIncrement()));
	connect(makeAction(ActionId::BLOCK_SKIP_REMOVE), SIGNAL(triggered()), this, SLOT(doBlockSkipRemove()));
	connect(makeAction(ActionId::CHAMFER), SIGNAL(triggered()), this, SLOT(doChamfer()));
	connect(makeAction(ActionId::CLEANUP), SIGNAL(triggered()), this, SLOT(doCleanUp()));
	connect(makeAction(ActionId::COMMENT_PARA), SIGNAL(triggered()), this, SLOT(doParaComment()));
	connect(makeAction(ActionId::COMMENT_SEMI), SIGNAL(triggered()), this, SLOT(doSemiComment()));
	connect(makeAction(ActionId::COMPILE_MACRO), SIGNAL(triggered()), this, SLOT(doCompileMacro()));
	connect(makeAction(ActionId::DOT), SIGNAL(triggered()), this, SLOT(doDot()));
	connect(makeAction(ActionId::EMPTY_LINE_INSERT), SIGNAL(triggered()), this, SLOT(doInsertEmptyLines()));
	connect(makeAction(ActionId::EMPTY_LINE_REMOVE), SIGNAL(triggered()), this, SLOT(doRemoveEmptyLines()));
	connect(makeAction(ActionId::FEEDS), SIGNAL(triggered()), this, SLOT(doFeeds()));
	connect(makeAction(ActionId::I2M), SIGNAL(triggered()), this, SLOT(doI2M()));
	connect(makeAction(ActionId::I2MPROG), SIGNAL(triggered()), this, SLOT(doI2MProg()));
	connect(makeAction(ActionId::RENUMBER), SIGNAL(triggered()), this, SLOT(doRenumber()));
	connect(makeAction(ActionId::SPACES_INSERT), SIGNAL(triggered()), this, SLOT(doInsertSpaces()));
	connect(makeAction(ActionId::SPACES_REMOVE), SIGNAL(triggered()), this, SLOT(doRemoveSpaces()));
	connect(makeAction(ActionId::SPLIT_PROGRAMMS), SIGNAL(triggered()), this, SLOT(doSplitProgramms()));
	connect(makeAction(ActionId::SWAP_AXES), SIGNAL(triggered()), this, SLOT(doSwapAxes()));
	connect(makeAction(ActionId::TRIANGLE), SIGNAL(triggered()), this, SLOT(doTriangle()));

	connect(parent, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(parent, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(parent, SIGNAL(updateShortcuts(const QMap<QString, QKeySequence>&)), this,
	        SLOT(loadShortcuts(const QMap<QString, QKeySequence>&)));

	Actions::loadIcons();
	Actions::loadTranslations();
}

Addons::Actions::~Actions()
{
}

void Addons::Actions::loadTranslations()
{
	setActionText(ActionId::BHC, tr("&Bolt hole circle"));
	setActionExToolTip(ActionId::BHC, tr("Calculate bolt hole's positions"));
	setActionText(ActionId::BLOCK_SKIP_DECREMENT, tr("Block Skip -"));
	setActionExToolTip(ActionId::BLOCK_SKIP_DECREMENT, tr("Insert/decrease Block Skip /"));
	setActionText(ActionId::BLOCK_SKIP_INCREMENT, tr("Block Skip +"));
	setActionExToolTip(ActionId::BLOCK_SKIP_INCREMENT, tr("Insert/increase Block Skip /"));
	setActionText(ActionId::BLOCK_SKIP_REMOVE, tr("Block Skip remove"));
	setActionExToolTip(ActionId::BLOCK_SKIP_REMOVE, tr("Remove Block Skip /"));
	setActionText(ActionId::CHAMFER, tr("Chamfer"));
	setActionExToolTip(ActionId::CHAMFER, tr("Calculate chamfer"));
	setActionText(ActionId::CLEANUP, tr("Clean &up"));
	setActionExToolTip(ActionId::CLEANUP, tr("Remove text using regular expressions"));
	setActionText(ActionId::COMMENT_PARA, tr("Comment ()"));
	setActionExToolTip(ActionId::COMMENT_PARA, tr("Comment/uncomment selected text using parentheses"));
	setActionText(ActionId::COMMENT_SEMI, tr("Comment ;"));
	setActionExToolTip(ActionId::COMMENT_SEMI, tr("Comment/uncomment selected text using semicolon"));
	setActionText(ActionId::COMPILE_MACRO, tr("Compile macro - experimental"));
	setActionExToolTip(ActionId::COMPILE_MACRO, tr("Translate EdytorNC macro into CNC program"));
	setActionText(ActionId::DOT, tr("Insert dots"));
	setActionExToolTip(ActionId::DOT, tr("Inserts decimal dot"));
	setActionText(ActionId::EMPTY_LINE_INSERT, tr("Insert empty lines"));
	setActionExToolTip(ActionId::EMPTY_LINE_INSERT, tr("Inserts empty lines"));
	setActionText(ActionId::EMPTY_LINE_REMOVE, tr("Remove empty lines"));
	setActionExToolTip(ActionId::EMPTY_LINE_REMOVE, tr("Removes empty lines"));
	setActionText(ActionId::FEEDS, tr("Feed's speed's"));
	setActionExToolTip(ActionId::FEEDS, tr("Calculate speed, feed, cutting speed"));
	setActionText(ActionId::I2M, tr("Convert inch <-> mm"));
	setActionExToolTip(ActionId::I2M, tr("Convert inch <-> mm"));
	setActionText(ActionId::I2MPROG, tr("Convert program inch <-> mm"));
	setActionExToolTip(ActionId::I2MPROG, tr("Convert program inch <-> mm"));
	setActionText(ActionId::RENUMBER, tr("Renumber"));
	setActionExToolTip(ActionId::RENUMBER, tr("Renumber program blocks"));
	setActionText(ActionId::SPACES_INSERT, tr("&Insert spaces"));
	setActionExToolTip(ActionId::SPACES_INSERT, tr("Inserts spaces"));
	setActionText(ActionId::SPACES_REMOVE, tr("Remove spaces"));
	setActionExToolTip(ActionId::SPACES_REMOVE, tr("Removes spaces"));
	setActionText(ActionId::SPLIT_PROGRAMMS, tr("Split file"));
	setActionExToolTip(ActionId::SPLIT_PROGRAMMS, tr("Split file"));
	setActionText(ActionId::SWAP_AXES, tr("Swap axes"));
	setActionExToolTip(ActionId::SWAP_AXES, tr("Swap/modify axes, selected text or entire program"));
	setActionText(ActionId::TRIANGLE, tr("Solution of triangles"));
	setActionExToolTip(ActionId::TRIANGLE, tr("Solution of triangles"));
}

void Addons::Actions::loadIcons()
{
	setActionIcon(ActionId::BHC, QIcon(":/images/bhc.png"));
	setActionIcon(ActionId::BLOCK_SKIP_DECREMENT, QIcon(":/images/blockskip-.png"));
	setActionIcon(ActionId::BLOCK_SKIP_INCREMENT, QIcon(":/images/blockskip+.png"));
	setActionIcon(ActionId::BLOCK_SKIP_REMOVE, QIcon(":/images/blockskipr.png"));
	setActionIcon(ActionId::CHAMFER, QIcon(":/images/chamfer.png"));
	setActionIcon(ActionId::CLEANUP, QIcon(":/images/cleanup.png"));
	setActionIcon(ActionId::COMMENT_PARA, QIcon(":/images/paracomment.pn"));
	setActionIcon(ActionId::COMMENT_SEMI, QIcon(":/images/semicomment.png"));
	setActionIcon(ActionId::COMPILE_MACRO, QIcon(":/images/compfile.png"));
	setActionIcon(ActionId::DOT, QIcon(":/images/dots.png"));
	setActionIcon(ActionId::EMPTY_LINE_INSERT, QIcon(":/images/insertemptylines.png"));
	setActionIcon(ActionId::EMPTY_LINE_REMOVE, QIcon(":/images/removeemptylines.png"));
	setActionIcon(ActionId::FEEDS, QIcon(":/images/vcf.png"));
	setActionIcon(ActionId::I2M, QIcon(":/images/i2m.png"));
	setActionIcon(ActionId::I2MPROG, QIcon(":/images/i2mprog.png"));
	setActionIcon(ActionId::RENUMBER, QIcon(":/images/renumber.png"));
	setActionIcon(ActionId::SPACES_INSERT, QIcon(":/images/insertspc.png"));
	setActionIcon(ActionId::SPACES_REMOVE, QIcon(":/images/removespc.png"));
	setActionIcon(ActionId::SPLIT_PROGRAMMS, QIcon(":/images/split_prog.png"));
	setActionIcon(ActionId::SWAP_AXES, QIcon(":/images/swapaxes.png"));
	setActionIcon(ActionId::TRIANGLE, QIcon(":/images/triangles.png"));
}

QAction* Addons::Actions::bhc()
{
	return action(ActionId::BHC);
}

QAction* Addons::Actions::blockSkipDecrement()
{
	return action(ActionId::BLOCK_SKIP_DECREMENT);
}

QAction* Addons::Actions::blockSkipIncrement()
{
	return action(ActionId::BLOCK_SKIP_INCREMENT);
}

QAction* Addons::Actions::blockSkipRemove()
{
	return action(ActionId::BLOCK_SKIP_REMOVE);
}

QAction* Addons::Actions::chamfer()
{
	return action(ActionId::CHAMFER);
}

QAction* Addons::Actions::cleanUp()
{
	return action(ActionId::CLEANUP);
}

QAction* Addons::Actions::paraComment()
{
	return action(ActionId::COMMENT_PARA);
}

QAction* Addons::Actions::semiComment()
{
	return action(ActionId::COMMENT_SEMI);
}

QAction* Addons::Actions::compileMacro()
{
	return action(ActionId::COMPILE_MACRO);
}

QAction* Addons::Actions::dot()
{
	return action(ActionId::DOT);
}

QAction* Addons::Actions::insertEmptyLines()
{
	return action(ActionId::EMPTY_LINE_INSERT);
}

QAction* Addons::Actions::removeEmptyLines()
{
	return action(ActionId::EMPTY_LINE_REMOVE);
}

QAction* Addons::Actions::feeds()
{
	return action(ActionId::FEEDS);
}

QAction* Addons::Actions::i2m()
{
	return action(ActionId::I2M);
}

QAction* Addons::Actions::i2mProg()
{
	return action(ActionId::I2MPROG);
}

QAction* Addons::Actions::renumber()
{
	return action(ActionId::RENUMBER);
}

QAction* Addons::Actions::insertSpaces()
{
	return action(ActionId::SPACES_INSERT);
}

QAction* Addons::Actions::removeSpaces()
{
	return action(ActionId::SPACES_REMOVE);
}

QAction* Addons::Actions::splitProgramms()
{
	return action(ActionId::SPLIT_PROGRAMMS);
}

QAction* Addons::Actions::swapAxes()
{
	return action(ActionId::SWAP_AXES);
}

QAction* Addons::Actions::triangle()
{
	return action(ActionId::TRIANGLE);
}

void Addons::Actions::doBhc()
{
	Addons::doBhc(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doBlockSkip(bool remove, bool inc)
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_BLOCKS)) {
		return;
	}

	Utils::blockSkip(ctx.text(), remove, inc);
	ctx.setSelectionStart(ctx.fragmentStart());
	ctx.setSelectionEnd(ctx.fragmentStart() + ctx.text().length());
	ctx.push();
}

void Addons::Actions::doBlockSkipDecrement()
{
	doBlockSkip(false, false);
}

void Addons::Actions::doBlockSkipIncrement()
{
	doBlockSkip(false, true);
}

void Addons::Actions::doBlockSkipRemove()
{
	doBlockSkip(true, true);
}

void Addons::Actions::doChamfer()
{
	Addons::doChamfer(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doCleanUp()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doCleanUp(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doParaComment()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Comments/uncomments"));

	bool changed = Utils::autoComments(ctx.text(), Utils::ParenthesisComments, [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doSemiComment()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Comments/uncomments"));

	bool changed = Utils::autoComments(ctx.text(), Utils::SemicolonComments, [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doCompileMacro()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	Utils::CompileMacro compiler;

	if (compiler.compile(ctx.text()) == -1) {
		QMessageBox::warning(m_app->mainWindow(), tr("GCodeWorkShop - compile macro"), compiler.status());
		//return;
	}

	GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(m_app->newFile());

	if (gdoc) {
		gdoc->insertText(compiler.result());
	}
}

void Addons::Actions::doDot()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doDot(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doInsertEmptyLines()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Inserting empty lines"));

	bool changed = Utils::insertEmptyLines(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doRemoveEmptyLines()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Removing empty lines"));

	bool changed = Utils::removeEmptyLines(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doFeeds()
{
	Addons::doFeeds(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doI2M()
{
	Addons::doI2M(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doI2MProg()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doI2MProg(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doRenumber()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_OR_ALL)) {
		return;
	}

	if (Addons::doRenumber(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doInsertSpaces()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{m_app->mainWindow()};
	helper.begin(ctx.text().length(), tr("Inserting space"));

	int changed = Utils::insertSpaces(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doRemoveSpaces()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{m_app->mainWindow()};
	helper.begin(ctx.text().length(), tr("Removing space"));

	int changed = Utils::removeSpaces(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doSplitProgramms()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	QApplication::setOverrideCursor(Qt::BusyCursor);

	QStringList list = Utils::splitFile(ctx.text());

	if (list.size() <= 1) {
		QApplication::restoreOverrideCursor();
		return;
	}

	for (QString it : list) {
		GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(m_app->newFile());

		if (gdoc == nullptr) {
			continue;
		}

		gdoc->setText(it);
		gdoc->clearUndoRedoStacks();
	}

	QApplication::restoreOverrideCursor();
}

void Addons::Actions::doSwapAxes()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_OR_ALL)) {
		return;
	}

	if (Addons::doSwapAxes(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doTriangle()
{
	Addons::doTriangle(m_app->mainWindow(), Medium::instance().settings());
}
