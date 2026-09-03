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
#include <QComboBox>            // for QComboBox
#include <QSettings>            // for QSettings
#include <QVariant>             // for QVariant
#include <QWidget>              // for QWidget

#include <highlightmode.h>
#include <gcoderdocument.h> // for GCoderDocument
#include <gcodeworkshop.h>  // for GCodeWorkShop
#include <serialportconfigdialog.h>
#include <serialporttestdialog.h>
#include <serialtransmissiondialog.h>
#include <utils/medium.h>

#include "serialtoolbar.h"
#include "serialtoolbaractions.h"   //  for SerialToolBarActions


#define CFG_SECTION                 "SerialToolBar"
#define CFG_KEY_IGNORE_CASE         "IgnoreCase"
#define CFG_KEY_IGNORE_COMMENTS     "IgnoreComments"
#define CFG_KEY_RETURN_REACTION     "ReturnReaction"
#define CFG_KEY_TEXT_FOR_FIND       "TextForFind"
#define CFG_KEY_TEXT_FOR_REPLACE    "TextForReplace"
#define CFG_KEY_WHOLE_WORDS         "WholeWords"

#define CFG_VALUE_FIND_NEXT     "FindNext"
#define CFG_VALUE_NO            "No"
#define CFG_VALUE_REVERT_FOCUS  "RevertFocus"


GUI::SerialToolBar::SerialToolBar(GCodeWorkShop* app, QWidget* parent) :
	QToolBar(tr("Serial port toolbar"), parent),
	m_app(app),
	m_actions(new GUI::SerialToolBarActions(app))
{
	setObjectName("SerialToolBar");

	connect(m_actions->configPort(), &QAction::triggered, this, &SerialToolBar::configure);
	connect(m_actions->receive(), &QAction::triggered, this, &SerialToolBar::receive);
	connect(m_actions->send(), &QAction::triggered, this, &SerialToolBar::send);
	connect(m_actions->attachToDir(), &QAction::triggered, this, &SerialToolBar::attachToDir);
	connect(m_actions->deAttachToDir(), &QAction::triggered, this, &SerialToolBar::deAttachToDir);
	connect(m_actions->testPort(), &QAction::triggered, this, &SerialToolBar::testConfig);
	connect(m_actions->close(), &QAction::triggered, m_app, &GCodeWorkShop::closeSerialToolbar);
	connect(m_actions->runFileServer(), &QAction::triggered, m_app, &GCodeWorkShop::startSerialPortServer);

	m_configBox = new QComboBox();
	m_configBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_configBox->setDuplicatesEnabled(false);

	//addSeparator();
	addAction(m_actions->runFileServer());
	addAction(m_actions->attachToDir());
	addAction(m_actions->deAttachToDir());
	addSeparator();
	addAction(m_actions->testPort());
	addWidget(m_configBox);
	addAction(m_actions->configPort());
	addSeparator();
	addAction(m_actions->receive());
	addSeparator();
	addAction(m_actions->send());

	addSeparator();
	addAction(m_actions->close());

	loadTranslations();
	loadIcons();
}

GUI::SerialToolBar::~SerialToolBar()
{
}

bool GUI::SerialToolBar::selectProfile(const QString& name)
{
	int id = m_configBox->findText(name);

	if (id >= 0) {
		m_configBox->setCurrentIndex(id);
		return true;
	} else {
		return false;
	}
}

GUI::Actions::ActionKit* GUI::SerialToolBar::actions() const
{
	return m_actions;
}

void GUI::SerialToolBar::loadTranslations()
{
	m_actions->loadTranslations();
}

void GUI::SerialToolBar::loadIcons()
{
	m_actions->loadIcons();
}

void GUI::SerialToolBar::loadSettings(QSettings* cfg)
{
}

void GUI::SerialToolBar::saveSettings(QSettings* cfg) const
{
}


void GUI::SerialToolBar::loadSerialConfignames()
{
	QSettings* cfg = Medium::instance().settings();
	cfg->beginGroup("SerialPortConfigs");

	QStringList profilesList = cfg->value("SettingsList", QStringList(tr("Default"))).toStringList();
	profilesList.sort();
	m_configBox->clear();
	m_configBox->addItems(profilesList);
	const QString& selected = cfg->value("CurrentSerialPortSettings", tr("Default")).toString();
	selectProfile(selected);
	cfg->endGroup();
}

void GUI::SerialToolBar::configure()
{
	SerialPortConfigDialog* serialConfigDialog = new SerialPortConfigDialog(m_app->mainWindow(),
	        m_configBox->currentText());

	if (serialConfigDialog->exec() == QDialog::Accepted) {
		loadSerialConfignames();
	}
}

void GUI::SerialToolBar::testConfig()
{
	SerialPortTestDialog* testDialog = new SerialPortTestDialog(m_app->mainWindow());
	testDialog->show();
}

void GUI::SerialToolBar::send()
{
	QString tx;
	GCoderDocument* gdoc = m_app->activeGCoderDocument();

	if (!gdoc) {
		return;
	}

	m_actions->receive()->setEnabled(false);
	m_actions->send()->setEnabled(false);
	m_actions->runFileServer()->setEnabled(false);
	QApplication::setOverrideCursor(Qt::BusyCursor);

	tx.append(gdoc->text());

	SerialTransmissionDialog transmissionDialog(m_app->mainWindow());
	transmissionDialog.sendData(tx, m_configBox->currentText());

	m_actions->receive()->setEnabled(true);
	m_actions->send()->setEnabled(true);
	m_actions->runFileServer()->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

void GUI::SerialToolBar::receive()
{
	m_actions->receive()->setEnabled(false);
	m_actions->send()->setEnabled(false);
	m_actions->runFileServer()->setEnabled(false);
	QApplication::setOverrideCursor(Qt::BusyCursor);

	SerialTransmissionDialog transmissionDialog(m_app->mainWindow());
	QStringList progList = transmissionDialog.receiveData(m_configBox->currentText());

	if (!progList.isEmpty()) {
		int id = m_configBox->currentIndex();

		QStringList::const_iterator it = progList.constBegin();

		if ((*it) == "#FILE_LIST#") {
			it++;

			while (it != progList.constEnd()) {
				m_app->openFile(*it);
				it++;
			}
		} else {
			if (!(*it).isEmpty() && !(*it).isNull()) {
				GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(m_app->newFile());

				if (gdoc) {
					gdoc->clear();
					gdoc->insertText(*it);
					gdoc->setHighlightMode(MODE_AUTO);
					//activeWindow->setReadOnly(defaultMdiWindowProperites.defaultReadOnly);
					gdoc->clearUndoRedoStacks();
				}
			}
		}

		m_configBox->setCurrentIndex(id);
	}

	m_actions->receive()->setEnabled(true);
	m_actions->send()->setEnabled(true);
	m_actions->runFileServer()->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

void GUI::SerialToolBar::attachToDir()
{
	m_app->attachSerialConfigToDir(m_configBox->currentText(), true);
}

void GUI::SerialToolBar::deAttachToDir()
{
	m_app->attachSerialConfigToDir("", false);
}
