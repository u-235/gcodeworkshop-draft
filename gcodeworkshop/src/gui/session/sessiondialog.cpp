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

#include <QList>            // for QList
#include <QListWidget>      // for QListWidget
#include <QListWidgetItem>  // for QListWidgetItem
#include <QPushButton>      // for QPushButton
#include <QString>          // for QString
#include <Qt>               // for WindowType

class QWidget;

#include "sessiondialog.h"
#include "sessionmanager.h"     // for SessionManager
#include "sessionnamedialog.h"  // for SessionNameDialog

#include "ui_sessiondialog.h"  // for Ui::SessionDialog


GUI::SessionDialog::SessionDialog(QWidget* parent, SessionManager* sessions) :
	QDialog(parent, Qt::Dialog),
	m_sessions(sessions),
	ui(new Ui::SessionDialog())
{
	ui->setupUi(this);
	setWindowTitle(tr("Session manager"));
	setModal(true);

	connect(ui->newPushButton, SIGNAL(clicked()), this, SLOT(newButtonClicked()));
	connect(ui->renamePushButton, SIGNAL(clicked()), this, SLOT(renameButtonClicked()));
	connect(ui->clonePushButton, SIGNAL(clicked()), this, SLOT(cloneButtonClicked()));
	connect(ui->deletePushButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
	connect(ui->switchPushButton, SIGNAL(clicked()), this, SLOT(switchButtonClicked()));

	connect(ui->sessionListWidget, SIGNAL(itemSelectionChanged()), this,
	        SLOT(sessionListItemSelectionChanged()));

	connect(m_sessions, SIGNAL(sessionListChanged(QStringList)), this, SLOT(updateSessionList(QStringList)));
	updateSessionList(m_sessions->sessionList());
}

GUI::SessionDialog::~SessionDialog()
{
}

void GUI::SessionDialog::newButtonClicked()
{
	GUI::SessionNameDialog* newSesDialog = new GUI::SessionNameDialog(this);
	newSesDialog->setName("");
	int result = newSesDialog->exec();

	if (result == QDialog::Accepted) {
		QString tx = newSesDialog->getName().simplified();

		if (!tx.isEmpty()) {
			m_sessions->addSession(tx);
		}
	}

	delete newSesDialog;
}

void GUI::SessionDialog::renameButtonClicked()
{
	QString currName = ui->sessionListWidget->currentItem()->text();
	GUI::SessionNameDialog* newSesDialog = new GUI::SessionNameDialog(this);
	newSesDialog->setName(currName);
	int result = newSesDialog->exec();

	if (result == QDialog::Accepted) {
		QString newName = newSesDialog->getName().simplified();

		if (!newName.isEmpty()) {
			m_sessions->renameSession(currName, newName);
		}
	}

	delete newSesDialog;
}

void GUI::SessionDialog::cloneButtonClicked()
{
	QString currName = ui->sessionListWidget->currentItem()->text();
	GUI::SessionNameDialog* newSesDialog = new GUI::SessionNameDialog(this);
	newSesDialog->setName(currName);
	int result = newSesDialog->exec();

	if (result == QDialog::Accepted) {
		QString newName = newSesDialog->getName().simplified();

		if (!newName.isEmpty()) {
			m_sessions->copySession(currName, newName);
		}
	}

	delete newSesDialog;
}

void GUI::SessionDialog::deleteButtonClicked()
{
	const QList<QListWidgetItem*>& selected = ui->sessionListWidget->selectedItems();

	if (!selected.isEmpty()) {
		m_sessions->removeSession(selected.at(0)->text());
	}
}

void GUI::SessionDialog::switchButtonClicked()
{
	m_sessions->setCurrentSession(ui->sessionListWidget->currentItem()->text());
	accept();
}

void GUI::SessionDialog::sessionListItemSelectionChanged()
{
	bool hasSelection = ui->sessionListWidget->selectedItems().size() == 1;
	ui->deletePushButton->setEnabled(hasSelection);
	ui->renamePushButton->setEnabled(hasSelection);
	ui->clonePushButton->setEnabled(hasSelection);
	ui->switchPushButton->setEnabled(hasSelection);
}

void GUI::SessionDialog::updateSessionList(const QStringList& list)
{
	ui->sessionListWidget->clear();
	ui->sessionListWidget->addItems(list);
}
