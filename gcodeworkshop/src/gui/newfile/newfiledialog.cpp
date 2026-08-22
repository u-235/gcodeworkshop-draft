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

#include <QApplication> // for QApplication
#include <QComboBox>    // for QComboBox
#include <QFile>        // for QFile
#include <QFileDialog>  // for QFileDialog
#include <QIcon>        //  for QIcon
#include <QLineEdit>    // for QLineEdit
#include <QPushButton>  // for QPushButton
#include <QSettings>    // for QSettings
#include <QStringList>  // for QStringList
#include <QVariant>     // for QVariant
#include <QtGlobal>     // for QFlags

#include <utils/medium.h>   // for Medium

#include "newfiledialog.h"
#include "ui_newfiledialog.h"   // for Ui::newFileDialog


// TODO: change a path to the file
#define TEMPLATE_PATH             "/usr/share/gcodeworkshop/TEMPLATE"


GUI::NewFileDialog::NewFileDialog(QWidget* parent) :
	QDialog(parent),
	m_filters("*.nc"),
	ui(new Ui::NewFileDialog)
{
	ui->setupUi(this);
	loadIcons();

	m_templatePath.setPath(TEMPLATE_PATH);

	if (!m_templatePath.exists()) {
		m_templatePath.setPath(QApplication::applicationDirPath() + "/" + "TEMPLATE");
	}

	if (!m_templatePath.exists()) {
		m_templatePath.setPath(QDir::homePath());
	}

	QSettings& settings = *Medium::instance().settings();

	QDir savedPath(settings.value("TemplatePath", m_templatePath.canonicalPath()).toString());

	if (savedPath.exists()) {
		m_templatePath = savedPath;
	}

	ui->pathLineEdit->setText(m_templatePath.canonicalPath());

	fillFileCombo();

	connect(ui->browsePushButton, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

	connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
}

GUI::NewFileDialog::~NewFileDialog()
{
	delete ui;
}

QString GUI::NewFileDialog::getChosenFile()
{
	QFile file(m_templatePath.canonicalPath() + "/" + ui->fileComboBox->currentText());

	if (file.exists()) {
		return file.fileName();
	} else {
		return tr("EMPTY FILE");
	}
}

void GUI::NewFileDialog::setNameFilters(const QStringList& filters)
{
	m_filters = filters;
	fillFileCombo();
}

void GUI::NewFileDialog::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::NewFileDialog::loadIcons()
{
	ui->browsePushButton->setIcon(QIcon(":/images/browse.png"));
}

void GUI::NewFileDialog::browseButtonClicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Choose template path"),
	                    m_templatePath.canonicalPath());

	if (!directory.isEmpty()) {
		m_templatePath.setPath(directory);
		ui->pathLineEdit->setText(m_templatePath.canonicalPath());
		fillFileCombo();
	}
}

void GUI::NewFileDialog::fillFileCombo()
{
	QStringList files = m_templatePath.entryList(m_filters,
	                    QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
	                    QDir::Name | QDir::IgnoreCase);
	ui->fileComboBox->clear();
	ui->fileComboBox->addItem(tr("EMPTY FILE"));
	ui->fileComboBox->addItems(files);
}

void GUI::NewFileDialog::saveSettings()
{
	QSettings& settings = *Medium::instance().settings();

	settings.setValue("TemplatePath", m_templatePath.canonicalPath());
}

int GUI::NewFileDialog::exec()
{
	return QDialog::exec();
}
