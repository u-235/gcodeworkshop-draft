/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of GCodeWorkShop.                                   *
 *                                                                         *
 *   GCodeWorkShop is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GUI_SESSIONDIALOG_H
#define GUI_SESSIONDIALOG_H

#include <QDialog>      // for QDialog
#include <QObject>      // for Q_OBJECT, slots
#include <QString>      // IWYU pragma: keep
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	#include <QStringList>
#else
	#include <QList>
	using QStringList = QList<QString>;
#endif

class QWidget;

class SessionManager;

namespace Ui {
class SessionDialog;
}


namespace GUI {
class SessionDialog: public QDialog
{
	Q_OBJECT

public:
	explicit SessionDialog(QWidget* parent, SessionManager* sessions);
	~SessionDialog();

private slots:
	void updateSessionList(const QStringList& list);
	void newButtonClicked();
	void renameButtonClicked();
	void deleteButtonClicked();
	void cloneButtonClicked();
	void switchButtonClicked();
	void sessionListItemSelectionChanged();

private:
	SessionManager* m_sessions;
	Ui::SessionDialog* ui;
};
} // namespace GUI

#endif // GUI_SESSIONDIALOG_H
