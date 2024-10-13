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

#include <ui/actions/actionkit.h>  // IWYU pragma: associated


Ui::Actions::ActionKit::ActionKit(QObject* parent) :
	QObject{parent},
	m_shortcutTipEnabled{true},
	m_actions{}
{
}

Ui::Actions::ActionKit::~ActionKit()
{
}

bool Ui::Actions::ActionKit::isShortcutTipEnabled() const
{
	return m_shortcutTipEnabled;
}

void Ui::Actions::ActionKit::setShortcutTipEnabled(bool enable)
{
	m_shortcutTipEnabled = enable;
}

void Ui::Actions::ActionKit::loadShortcuts(const QMap<QString, QKeySequence>& keymap)
{
	QMap<QString, ActionEx>::iterator end = m_actions.end();

	for (QMap<QString, ActionEx>::iterator i = m_actions.begin(); i != end; ++i) {
		setActionShortcut(i.key(), keymap.value(i.key()));
	}
}

QAction* Ui::Actions::ActionKit::makeAction(const QString& id)
{
	ActionEx ae{new QAction(this), QString()};
	m_actions.insert(id, ae);
	return ae.action;
}

QAction* Ui::Actions::ActionKit::action(const QString& id) const
{
	return m_actions.value(id).action;
}

void Ui::Actions::ActionKit::setActionIcon(const QString& id, const QIcon& icon)
{
	m_actions.value(id).action->setIcon(icon);
}

void Ui::Actions::ActionKit::setActionShortcut(const QString& id, const QKeySequence& shortcut)
{
	m_actions.value(id).action->setShortcut(shortcut);
	updateActionToolTip(id);
}

void Ui::Actions::ActionKit::setActionText(const QString& id, const QString& text)
{
	m_actions.value(id).action->setText(text);
}

void Ui::Actions::ActionKit::setActionExToolTip(const QString& id, const QString& tooltip)
{
	m_actions[id].tooltip = tooltip;
	updateActionToolTip(id);
}

QString Ui::Actions::ActionKit::actionExToolTip(const QString& id) const
{
	return m_actions.value(id).tooltip;
}

void Ui::Actions::ActionKit::updateActionToolTip(const QString& id)
{
	QAction* act = m_actions[id].action;
	QString tooltip = actionExToolTip(id);

	if (m_shortcutTipEnabled && !act->shortcut().isEmpty()) {
		tooltip.append((tr("<p>Shortcut: <b>%1</b>")).arg(act->shortcut().toString()));
	}

	act->setToolTip(tooltip);
}
