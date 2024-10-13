/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
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
 *  but WITHOUT ANY WARRANTY(new QAction(this)), without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QAction>              // for QAction
#include <QApplication>         // for QApplication
#include <QCheckBox>            // for QCheckBox
#include <QColor>               // for QColor
#include <QLabel>               // for QLabel
#include <QLineEdit>            // for QLineEdit
#include <QPalette>             // for QPalette, QPalette::Base
#include <QRegularExpression>   // for QRegularExpression
#include <QSettings>            // for QSettings
#include <QVariant>             // for QVariant
#include <QWidget>              // for QWidget
#include <Qt>                   // for MouseFocusReason, red, BusyCursor
#include <QtGlobal>             // for Q_UNUSED

#include <gcoderdocument.h>   // for GCoderDocument
#include <gcodeworkshop.h>    // for GCodeWorkShop

#include "capslockeventfilter.h"    // for CapsLockEventFilter
#include "findtoolbar.h"
#include "findtoolbaractions.h"     //  for FindToolBarActions


#define CFG_SECTION                 "FindToolBar"
#define CFG_KEY_IGNORE_CASE         "IgnoreCase"
#define CFG_KEY_IGNORE_COMMENTS     "IgnoreComments"
#define CFG_KEY_RETURN_REACTION     "ReturnReaction"
#define CFG_KEY_TEXT_FOR_FIND       "TextForFind"
#define CFG_KEY_TEXT_FOR_REPLACE    "TextForReplace"
#define CFG_KEY_WHOLE_WORDS         "WholeWords"

#define CFG_VALUE_FIND_NEXT     "FindNext"
#define CFG_VALUE_NO            "No"
#define CFG_VALUE_REVERT_FOCUS  "RevertFocus"


Ui::FindToolBar::FindToolBar(::GCodeWorkShop* parent) :
	QToolBar(parent),
	m_app(parent),
	m_actions(new Ui::FindToolBarActions(parent)),
	m_returnReaction(NO),
	m_findLabel(new QLabel()),
	m_replaceLabel(new QLabel()),
	m_findEdit(new QLineEdit()),
	m_replaceEdit(new QLineEdit()),
	m_wholeWords(new QCheckBox()),
	m_ignoreCase(new QCheckBox()),
	m_ignoreComments(new QCheckBox())
{
	connect(m_app, SIGNAL(updateTranslations()), this, SLOT(loadTranslations()));
	connect(m_app, SIGNAL(updateIcons()), this, SLOT(loadIcons()));
	connect(m_app, SIGNAL(intCapsLockChanged(bool)), this, SLOT(setCapsLockEnable(bool)));

	connect(m_actions->findNext(), SIGNAL(triggered()), this, SLOT(findNext()));
	connect(m_actions->findPrevious(), SIGNAL(triggered()), this, SLOT(findPrevious()));
	connect(m_actions->replaceNext(), SIGNAL(triggered()), this, SLOT(replaceNext()));
	connect(m_actions->replacePrevious(), SIGNAL(triggered()), this, SLOT(replacePrevious()));
	connect(m_actions->replaceAll(), SIGNAL(triggered()), this, SLOT(replaceAll()));
	connect(m_actions->findClose(), SIGNAL(triggered()), this, SLOT(close()));
	connect(m_findEdit, SIGNAL(textChanged(const QString&)), this, SLOT(findTextChanged()));
	connect(m_findEdit, SIGNAL(returnPressed()), this, SLOT(findReturnPressed()));
	connect(m_ignoreCase, SIGNAL(clicked()), this, SLOT(findTextChanged()));
	connect(m_wholeWords, SIGNAL(clicked()), this, SLOT(findTextChanged()));
	connect(m_ignoreComments, SIGNAL(clicked()), this, SLOT(findTextChanged()));

	m_findEdit->setClearButtonEnabled(true);
	m_findEditEventFilter = new CapsLockEventFilter(m_findEdit);
	m_findEditEventFilter->setCapsLockEnable(false);
	m_findEdit->installEventFilter(m_findEditEventFilter);

	m_replaceEdit->setClearButtonEnabled(true);
	m_replaceEditEventFilter = new CapsLockEventFilter(m_replaceEdit);
	m_replaceEditEventFilter->setCapsLockEnable(false);
	m_replaceEdit->installEventFilter(m_replaceEditEventFilter);

	m_wholeWords->setChecked(false);
	m_ignoreCase->setChecked(true);
	m_ignoreComments->setChecked(true);

	addWidget(m_findLabel);
	addWidget(m_findEdit);
	addAction(m_actions->findPrevious());
	addAction(m_actions->findNext());
	addSeparator();
	addWidget(m_replaceLabel);
	addWidget(m_replaceEdit);
	addAction(m_actions->replacePrevious());
	addAction(m_actions->replaceNext());
	addAction(m_actions->replaceAll());
	addSeparator();
	addWidget(m_ignoreCase);
	addWidget(m_wholeWords);
	//addSeparator();
	addWidget(m_ignoreComments);
	addSeparator();
	addAction(m_actions->findClose());

	loadTranslations();
	loadIcons();
}

Ui::FindToolBar::~FindToolBar()
{
}

bool Ui::FindToolBar::findNext()
{
	return findInDocument(m_app->activeGCoderDocument(), false);
}

bool Ui::FindToolBar::findPrevious()
{
	return findInDocument(m_app->activeGCoderDocument(), true);
}

void Ui::FindToolBar::replaceNext()
{
	replaceInDocument(m_app->activeGCoderDocument(), false, false);
}

void Ui::FindToolBar::replacePrevious()
{
	replaceInDocument(m_app->activeGCoderDocument(), true, false);
}

void Ui::FindToolBar::replaceAll()
{
	QApplication::setOverrideCursor(Qt::BusyCursor);
	replaceInDocument(m_app->activeGCoderDocument(), false, true);
	QApplication::restoreOverrideCursor();
}

void Ui::FindToolBar::highlight()
{
	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (!gdoc) {
		return;
	}

	if (!isHidden()) {
		gdoc->highlightFindText(textForFind(), isWholeWords(), !isIgnoreCase(), isIgnoreComments());
	} else {
		gdoc->highlightFindText("");
	}
}

void Ui::FindToolBar::activate(bool replace)
{
	Q_UNUSED(replace)
	QString lookFor;
	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (gdoc) {
		if (gdoc->hasSelection()) {
			lookFor = gdoc->selectedText();
		} else {
			lookFor = gdoc->wordUnderCursor();
		}

		if (!lookFor.isEmpty()) {
			setTextForFind(lookFor);
		}

		gdoc->highlightFindText(textForFind(), isWholeWords(), isIgnoreCase(), isIgnoreComments());
	}
}

void Ui::FindToolBar::deActivate()
{
	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (gdoc) {
		gdoc->widget()->setFocus(Qt::MouseFocusReason);
		gdoc->highlightFindText("");
		gdoc->centerCursor();
	}
}

void Ui::FindToolBar::setReturnReaction(ReturnReaction reaction)
{
	m_returnReaction = reaction;
}

Ui::FindToolBar::ReturnReaction Ui::FindToolBar::returnReaction() const
{
	return m_returnReaction;
}

bool Ui::FindToolBar::findInDocument(GCoderDocument* gdoc, bool backward)
{
	if (!gdoc || gdoc->isReadOnly()) {
		return false;
	}

	QPalette palette = QApplication::palette(m_findEdit);
	bool found = gdoc->findNext(textForFind(), isWholeWords(), isIgnoreCase(), isIgnoreComments(), backward);

	if (!found) {
		palette.setColor(QPalette::Base, QColor(Qt::red).lighter(160));
	}

	m_findEdit->setPalette(palette);
	return found;
}

bool Ui::FindToolBar::replaceInDocument(GCoderDocument* gdoc, bool backward, bool all)
{
	if (!gdoc || gdoc->isReadOnly()) {
		return false;
	}

	QPalette palette = QApplication::palette(m_findEdit);
	bool found;

	if (all) {
		found = gdoc->replaceAll(textForFind(), textForReplace(), isWholeWords(), isIgnoreCase(), isIgnoreComments());
	} else {
		found = gdoc->replaceNext(textForFind(), textForReplace(), isWholeWords(), isIgnoreCase(), isIgnoreComments(),
		                          backward);
	}

	if (!found) {
		palette.setColor(QPalette::Base, QColor(Qt::red).lighter(160));
	}

	m_findEdit->setPalette(palette);
	return found;
}

void Ui::FindToolBar::hideEvent(QHideEvent* event)
{
	deActivate();
	QWidget::hideEvent(event);
}

void Ui::FindToolBar::closeEvent(QCloseEvent* event)
{
	deActivate();
	QWidget::closeEvent(event);
}

void Ui::FindToolBar::findReturnPressed()
{
	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (!gdoc) {
		return;
	}

	switch (m_returnReaction) {
	case REVERT_FOCUS:
		gdoc->widget()->setFocus(Qt::OtherFocusReason);
		gdoc->centerCursor();
		break;

	case FIND_NEXT:
		findNext();
		break;

	default:
		;
	}
}

void Ui::FindToolBar::findTextChanged()
{
	if (textForFind().contains(QRegularExpression("\\$\\$"))
	        || textForFind().contains(QRegularExpression("(\\$)[-]{0,1}[0-9]{0,}[0-9.]{1,1}[0-9]{0,}"))) {
		m_actions->replaceAll()->setEnabled(false);
	} else {
		m_actions->replaceAll()->setEnabled(true);
	}

	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (gdoc) {
		gdoc->clearSelection(true);
		highlight();

		if (!textForFind().isEmpty()) {
			findNext();
		} else {
			m_findEdit->setPalette(QPalette());
		}
	}
}

void Ui::FindToolBar::setCapsLockEnable(bool enable)
{
	m_findEditEventFilter->setCapsLockEnable(enable);
	m_replaceEditEventFilter->setCapsLockEnable(enable);
}

void Ui::FindToolBar::loadTranslations()
{
	setWindowTitle(tr("Find"));

	m_findLabel->setText(tr("Find:"));
	m_replaceLabel->setText(tr("Replace with:"));
	m_findEdit->setToolTip(
	    tr("<b>Letter$$</b> - matches any number.<p><b>Letter$max$min</b> - matches number &lt;=max &gt;=min.</p>" \
	       "<p><b>$min</b> can be ommited, then equal 0</p>" \
	       "<p><b>X$100$-10</b> - matches all X with value -10 to 100</p>"));
	m_replaceEdit->setToolTip(
	    tr("<b>$$OperatorNumber</b> - do some math on replaced numbers. Operator +-*/" \
	       "<p>$$+1 - will add 1 to replaced numbers</p>"));
	m_wholeWords->setText(tr("&Whole words only"));
	m_ignoreCase->setText(tr("Ignore c&ase"));
	m_ignoreComments->setText(tr("Ignore c&omments"));
}

void Ui::FindToolBar::loadIcons()
{
}

void Ui::FindToolBar::loadSettings(QSettings* cfg)
{
	cfg->beginGroup(CFG_SECTION);
	m_ignoreCase->setChecked(cfg->value(CFG_KEY_IGNORE_CASE, m_ignoreCase->isChecked()).toBool());
	m_ignoreComments->setChecked(cfg->value(CFG_KEY_IGNORE_COMMENTS, m_ignoreComments->isChecked()).toBool());
	QString rr = cfg->value(CFG_KEY_RETURN_REACTION, "").toString();
	m_findEdit->setText(cfg->value(CFG_KEY_TEXT_FOR_FIND, "").toString());
	m_replaceEdit->setText(cfg->value(CFG_KEY_TEXT_FOR_REPLACE, "").toString());
	m_wholeWords->setChecked(cfg->value(CFG_KEY_WHOLE_WORDS, m_wholeWords->isChecked()).toBool());
	cfg->endGroup();

	if (rr == CFG_VALUE_FIND_NEXT) {
		m_returnReaction = FIND_NEXT;
	} else if (rr == CFG_VALUE_REVERT_FOCUS) {
		m_returnReaction = REVERT_FOCUS;
	} else {
		m_returnReaction = NO;
	}
}

void Ui::FindToolBar::saveSettings(QSettings* cfg) const
{
	QString rr;

	switch (m_returnReaction) {
	case REVERT_FOCUS:
		rr = CFG_VALUE_REVERT_FOCUS;
		break;

	case FIND_NEXT:
		rr = CFG_VALUE_FIND_NEXT;
		break;

	default:
		rr = CFG_VALUE_NO;
	}

	cfg->beginGroup(CFG_SECTION);
	cfg->setValue(CFG_KEY_IGNORE_CASE, m_ignoreCase->isChecked());
	cfg->setValue(CFG_KEY_IGNORE_COMMENTS, m_ignoreComments->isChecked());
	cfg->setValue(CFG_KEY_RETURN_REACTION, rr);
	cfg->setValue(CFG_KEY_TEXT_FOR_FIND, m_findEdit->text());
	cfg->setValue(CFG_KEY_TEXT_FOR_REPLACE, m_replaceEdit->text());
	cfg->setValue(CFG_KEY_WHOLE_WORDS, m_wholeWords->isChecked());
	cfg->endGroup();
}

Ui::Actions::ActionKit* Ui::FindToolBar::actions() const
{
	return m_actions;
}

bool Ui::FindToolBar::isWholeWords() const
{
	return m_wholeWords->isChecked();
}

bool Ui::FindToolBar::isIgnoreCase() const
{
	return m_ignoreCase->isChecked();
}

bool Ui::FindToolBar::isIgnoreComments() const
{
	return m_ignoreComments->isChecked();
}

QString Ui::FindToolBar::textForFind() const
{
	return m_findEdit->text();
}

void Ui::FindToolBar::setTextForFind(const QString& text)
{
	if (text.size() < 32) {
		m_findEdit->setText(text);
	}

	m_findEdit->setPalette(QPalette());
	m_findEdit->setFocus(Qt::MouseFocusReason);
	m_findEdit->selectAll();
}

QString Ui::FindToolBar::textForReplace() const
{
	return m_replaceEdit->text();
}
