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

#include <QAction>      // for QAction

#include <gui/actions/actionkit.h>  // IWYU pragma: associated


bool GUI::Actions::ActionKit::shortcutHintEnabled = true;

GUI::Actions::ActionKit::ActionKit(QObject* parent) :
	QObject{parent},
	m_actions{}
{
}

GUI::Actions::ActionKit::~ActionKit()
{
}

bool GUI::Actions::ActionKit::isShortcutHintEnabled()
{
	return shortcutHintEnabled;
}

void GUI::Actions::ActionKit::setShortcutHintEnabled(bool enable)
{
	shortcutHintEnabled = enable;
}

void GUI::Actions::ActionKit::loadShortcuts(const QMap<QString, QKeySequence>& keymap)
{
	QMap<QString, ActionEx>::iterator end = m_actions.end();

	for (QMap<QString, ActionEx>::iterator i = m_actions.begin(); i != end; ++i) {
		setActionShortcut(i.key(), keymap.value(i.key()));
	}
}

QAction* GUI::Actions::ActionKit::makeAction(const QString& id)
{
	ActionEx ae{new QAction(this), QString()};
	m_actions.insert(id, ae);
	return ae.action;
}

QAction* GUI::Actions::ActionKit::action(const QString& id) const
{
	return m_actions.value(id).action;
}

void GUI::Actions::ActionKit::setActionIcon(const QString& id, const QIcon& icon)
{
	m_actions.value(id).action->setIcon(icon);
}

void GUI::Actions::ActionKit::setActionShortcut(const QString& id, const QKeySequence& shortcut)
{
	m_actions.value(id).action->setShortcut(shortcut);
	updateActionToolTip(id);
}

void GUI::Actions::ActionKit::setActionText(const QString& id, const QString& text)
{
	m_actions.value(id).action->setText(text);
}

void GUI::Actions::ActionKit::setActionExToolTip(const QString& id, const QString& tooltip)
{
	m_actions[id].tooltip = tooltip;
	updateActionToolTip(id);
}

QString GUI::Actions::ActionKit::actionExToolTip(const QString& id) const
{
	return m_actions.value(id).tooltip;
}

void GUI::Actions::ActionKit::updateActionToolTip(const QString& id)
{
	QAction* act = m_actions[id].action;
	QString tooltip = actionExToolTip(id);

	if (shortcutHintEnabled && !act->shortcut().isEmpty()) {
		tooltip.append((tr("<p>Shortcut: <b>%1</b>")).arg(act->shortcut().toString()));
	}

	act->setToolTip(tooltip);
}
