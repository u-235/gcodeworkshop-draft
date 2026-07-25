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

#ifndef GUI_ACTIONS_ACTIONKIT_H
#define GUI_ACTIONS_ACTIONKIT_H

#include <QKeySequence> // for QKeySequence
#include <QMap>         // for QMap
#include <QObject>      // for QObject, Q_OBJECT, slots
#include <QString>      // IWYU pragma: keep

class QAction;
class QIcon;


namespace GUI {
namespace Actions {
class ActionKit : public QObject
{
	Q_OBJECT

public:
	explicit ActionKit(QObject* parent);
	virtual ~ActionKit();

	struct ActionEx {
		QAction* action;
		QString tooltip;
	};

	static bool isShortcutHintEnabled();
	static void setShortcutHintEnabled(bool enable);

public slots:
	virtual void loadTranslations() = 0;
	virtual void loadIcons() = 0;
	void loadShortcuts(const QMap<QString, QKeySequence>& keymap);

protected:
	QAction* makeAction(const QString& id);
	QAction* action(const QString& id) const;
	void setActionIcon(const QString& id, const QIcon& icon);
	void setActionShortcut(const QString& id, const QKeySequence& shortcut);
	void setActionText(const QString& id, const QString& text);
	void setActionExToolTip(const QString& id, const QString& tooltip);
	QString actionExToolTip(const QString& id) const;

private:
	void updateActionToolTip(const QString& id);

	static bool shortcutHintEnabled;
	QMap<QString, ActionEx> m_actions;
};
} // namespace Actions
} // namespace GUI

#endif // GUI_ACTIONS_ACTIONKIT_H
