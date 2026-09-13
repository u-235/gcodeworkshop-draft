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

#include <QButtonGroup>                 // for QButtonGroup
#include <QCheckBox>                    // for QCheckBox
#include <QComboBox>                    // for QComboBox
#include <QCoreApplication>             // for QCoreApplication
#include <QDesktopServices>             // for QDesktopServices
#include <QDir>                         // for QDir
#include <QDoubleSpinBox>               // for QDoubleSpinBox
#include <QFileDialog>                  // for QFileDialog
#include <QFileInfo>                    // for QFileInfo
#include <QIcon>                        // for QIcon
#include <QIntValidator>                // for QIntValidator
#include <QLineEdit>                    // for QLineEdit
#include <QList>                        // for QList
#include <QListWidget>                  // for QListWidget
#include <QPalette>                     // for QPalette
#include <QPushButton>                  // for QPushButton
#include <QRegularExpression>           // for QRegularExpression
#include <QRegularExpressionValidator>  // for QRegularExpressionValidator
#include <QSerialPort>                  // for QSerialPort
#include <QSerialPortInfo>              // for QSerialPortInfo
#include <QSettings>                    // for QSettings
#include <QSpinBox>                     // for QSpinBox
#include <QString>                      // for QString
#include <QStringList>                  // for QStringList
#include <QTabWidget>                   // for QTabWidget
#include <QToolButton>                  // for QToolButton
#include <QUrl>                         // for QUrl
#include <QVariant>                     // for QVariant
#include <QtGlobal>                     // for QForeachContainer, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, Q_OS_...

class QWidget;

#include <utils/medium.h>   // for Medium

#include <gui/serialport/serialportconfigdialog.h>

#include "ui_serialportconfigdialog.h"  // for Ui::SerialPortConfigDialog


// TODO: move into class SerialPortConfigDialog
static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

GUI::SerialPortConfigDialog::SerialPortConfigDialog(QWidget* parent, QString confName,
        Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::SerialPortConfigDialog())
{
	ui->setupUi(this);
	loadIcons();
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Serial port configuration"));
	setModal(true);

	configName = confName;

	baudGroup = new QButtonGroup(this);
	baudGroup->setExclusive(true);
	baudGroup->addButton(ui->b1CheckBox, 300);
	baudGroup->addButton(ui->b2CheckBox, 600);
	baudGroup->addButton(ui->b3CheckBox, QSerialPort::Baud1200);
	baudGroup->addButton(ui->b4CheckBox, QSerialPort::Baud2400);
	baudGroup->addButton(ui->b5CheckBox, QSerialPort::Baud4800);
	baudGroup->addButton(ui->b6CheckBox, QSerialPort::Baud9600);
	baudGroup->addButton(ui->b7CheckBox, QSerialPort::Baud19200);
	baudGroup->addButton(ui->b8CheckBox, QSerialPort::Baud38400);
	baudGroup->addButton(ui->b9CheckBox, QSerialPort::Baud57600);
	baudGroup->addButton(ui->b11CheckBox, QSerialPort::Baud115200);

	dataBitsGroup = new QButtonGroup(this);
	dataBitsGroup->setExclusive(true);
	dataBitsGroup->addButton(ui->d1CheckBox, QSerialPort::Data5);
	dataBitsGroup->addButton(ui->d2CheckBox, QSerialPort::Data6);
	dataBitsGroup->addButton(ui->d3CheckBox, QSerialPort::Data7);
	dataBitsGroup->addButton(ui->d4CheckBox, QSerialPort::Data8);

	stopBitsGroup = new QButtonGroup(this);
	stopBitsGroup->setExclusive(true);
	stopBitsGroup->addButton(ui->s1CheckBox, QSerialPort::OneStop);
	stopBitsGroup->addButton(ui->s2CheckBox, QSerialPort::TwoStop);

	parityGroup = new QButtonGroup(this);
	parityGroup->setExclusive(true);
	parityGroup->addButton(ui->p1CheckBox, QSerialPort::NoParity);
	parityGroup->addButton(ui->p2CheckBox, QSerialPort::OddParity);
	parityGroup->addButton(ui->p3CheckBox, QSerialPort::EvenParity);

	flowCtlGroup = new QButtonGroup(this);
	flowCtlGroup->setExclusive(true);
	flowCtlGroup->addButton(ui->f1CheckBox, QSerialPort::NoFlowControl);
	flowCtlGroup->addButton(ui->f2CheckBox, QSerialPort::HardwareControl);
	flowCtlGroup->addButton(ui->f3CheckBox, QSerialPort::SoftwareControl);

#ifdef Q_OS_WIN32
	ui->browseButton->setEnabled(false);
#else
	connect(ui->browseButton, &QPushButton::clicked, this, &SerialPortConfigDialog::browseButtonClicked);
#endif

	connect(ui->browse1PushButton, &QPushButton::clicked, this, &SerialPortConfigDialog::browse1ButtonClicked);
	connect(ui->browse2PushButton, &QPushButton::clicked, this, &SerialPortConfigDialog::browse2ButtonClicked);
	connect(ui->browse3PushButton, &QPushButton::clicked, this, &SerialPortConfigDialog::browse3ButtonClicked);
	connect(ui->browse4PushButton, &QPushButton::clicked, this, &SerialPortConfigDialog::browse4ButtonClicked);

	connect(ui->saveButton, &QPushButton::clicked, this, &SerialPortConfigDialog::saveButtonClicked);
	connect(ui->saveCloseButton, &QPushButton::clicked, this, &SerialPortConfigDialog::saveCloseButtonClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &SerialPortConfigDialog::deleteButtonClicked);
	connect(ui->closeButton, &QPushButton::clicked, this, &SerialPortConfigDialog::closeButtonClicked);
	connect(ui->helpButton, &QPushButton::clicked, this, &SerialPortConfigDialog::helpButtonClicked);

	connect(ui->addButton, &QPushButton::clicked, this, &SerialPortConfigDialog::addButtonClicked);
	connect(ui->removeButton, &QPushButton::clicked, this, &SerialPortConfigDialog::removeButtonClicked);

	connect(ui->addEobButton, &QToolButton::clicked, this, &SerialPortConfigDialog::addEobButtonClicked);
	connect(ui->deleteEobButton, &QToolButton::clicked, this, &SerialPortConfigDialog::deleteEobButtonClicked);

	connect(ui->addfileNameButton, &QToolButton::clicked, this, &SerialPortConfigDialog::addFileNameButtonClicked);
	connect(ui->removeFileNameButton, &QToolButton::clicked, this, &SerialPortConfigDialog::removeFileNameButtonClicked);

	connect(ui->addEobCharButton, &QToolButton::clicked, this, &SerialPortConfigDialog::addEobCharButtonClicked);
	connect(ui->deleteEobCharButton, &QToolButton::clicked, this, &SerialPortConfigDialog::deleteEobCharButtonClicked);
	connect(ui->eobComboBox, &QComboBox::editTextChanged, this, &SerialPortConfigDialog::eobComboBoxEditTextChanged);

	connect(ui->autoSaveCheckBox, &QCheckBox::stateChanged, this, &SerialPortConfigDialog::autoSaveCheckBoxChanged);
	connect(ui->fileServerCheckBox, &QCheckBox::stateChanged, this, &SerialPortConfigDialog::fileServerCheckBoxChanged);
	connect(ui->appendExtCheckBox, &QCheckBox::stateChanged, this, &SerialPortConfigDialog::appendExtCheckBoxChanged);
	connect(ui->useAsExtCheckBox, &QCheckBox::stateChanged, this, &SerialPortConfigDialog::useAsExtCheckBoxChanged);

	connect(ui->searchPath1LineEdit, &QLineEdit::textChanged, this, &SerialPortConfigDialog::readPath1Changed);
	connect(ui->searchPath2LineEdit, &QLineEdit::textChanged, this, &SerialPortConfigDialog::readPath2Changed);
	connect(ui->searchPath3LineEdit, &QLineEdit::textChanged, this, &SerialPortConfigDialog::readPath3Changed);

	//connect(flowCtlGroup, SIGNAL(buttonReleased(int)), &SerialPortConfigDialog::flowCtlGroupReleased()));

	loadSettings();
	flowCtlGroupReleased();

	connect(ui->configNameBox, &QComboBox::currentTextChanged, this, &SerialPortConfigDialog::changeSettings);
	connect(ui->portNameComboBox, &QComboBox::currentTextChanged, this,
	        &SerialPortConfigDialog::portNameComboBoxIndexChanged);

	ui->tabWidget->setCurrentIndex(0);

	QRegularExpression rx("(LF|CR){1,6}");
	rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	ui->eobComboBox->setValidator(new QRegularExpressionValidator(rx, this));

	ui->xonInput->setValidator(new QIntValidator(0, 127, this));

	ui->xoffInput->setValidator(new QIntValidator(0, 127, this));

	setResult(QDialog::Rejected);
}

GUI::SerialPortConfigDialog::~SerialPortConfigDialog()
{
	delete ui;
}

void GUI::SerialPortConfigDialog::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::SerialPortConfigDialog::loadIcons()
{
	QIcon addIcon(":/images/add.png");
	ui->addEobButton->setIcon(addIcon);
	ui->addEobCharButton->setIcon(addIcon);
	ui->addfileNameButton->setIcon(addIcon);
	ui->addButton->setIcon(addIcon);
	QIcon browseIcon(":/images/browse.png");
	ui->browseButton->setIcon(browseIcon);
	ui->browse1PushButton->setIcon(browseIcon);
	ui->browse2PushButton->setIcon(browseIcon);
	ui->browse4PushButton->setIcon(browseIcon);
	ui->browse3PushButton->setIcon(browseIcon);
	ui->closeButton->setIcon(QIcon(":/images/cancel.png"));
	QIcon deleteIcon(":/images/editdelete.png");
	ui->deleteButton->setIcon(deleteIcon);
	QIcon removeIcon(":/images/list-remove.png");
	ui->deleteEobButton->setIcon(removeIcon);
	ui->deleteEobCharButton->setIcon(removeIcon);
	ui->removeFileNameButton->setIcon(removeIcon);
	ui->removeButton->setIcon(removeIcon);
	QIcon saveIcon(":/images/filesave.png");
	ui->saveButton->setIcon(saveIcon);
	ui->saveCloseButton->setIcon(saveIcon);
}

void GUI::SerialPortConfigDialog::flowCtlGroupReleased()
{
}

void GUI::SerialPortConfigDialog::browseButtonClicked()
{
	QString fileName = QFileDialog::getOpenFileName(
	                       this,
	                       tr("Select serial port device"),
	                       "/dev/",
	                       tr("All files (*)"));

	QFileInfo file(fileName);

	if ((file.exists()) && (file.isReadable())) {
		ui->portNameComboBox->addItem(fileName);
		ui->portNameComboBox->setCurrentIndex(ui->portNameComboBox->count() - 1);
	}
}

void GUI::SerialPortConfigDialog::saveButtonClicked()
{
	QStringList list;
	QString item, curItem;

	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("SerialPortConfigs");

	curItem = ui->configNameBox->currentText();

	//#ifndef Q_OS_WIN32
	//    list.clear();
	//    list.prepend(ui->portNameComboBox->currentText());
	//    for(int i = 0; i <= ui->portNameComboBox->count(); i++)
	//    {
	//       item = ui->portNameComboBox->itemText(i);
	//       if(!item.isEmpty())
	//         if(!list.contains(item))
	//           list.prepend(item);
	//    }

	//    while(list.size() > 64)
	//    {
	//       list.removeLast();
	//    }
	//    settings.setValue("PortNameList", list);
	//#endif

	list.clear();
	list.prepend(ui->configNameBox->currentText());

	for (int i = 0; i <= ui->configNameBox->count(); i++) {
		item = ui->configNameBox->itemText(i);

		if (!item.isEmpty())
			if (!list.contains(item)) {
				list.prepend(item);
			}
	}

	while (list.size() > 64) {
		list.removeLast();
	}

	list.sort();
	settings.setValue("SettingsList", list);
	settings.setValue("CurrentSerialPortSettings", ui->configNameBox->currentText());

	settings.beginGroup(ui->configNameBox->currentText());

	settings.setValue("PortName", ui->portNameComboBox->currentText());
	settings.setValue("BaudRate", baudGroup->checkedId());
	settings.setValue("DataBits", dataBitsGroup->checkedId());
	settings.setValue("StopBits", stopBitsGroup->checkedId());
	settings.setValue("Parity", parityGroup->checkedId());
	settings.setValue("FlowControl", flowCtlGroup->checkedId());
	settings.setValue("SendAtEnd", ui->eotInput->text());
	settings.setValue("SendAtBegining", ui->stInput->text());
	settings.setValue("LineDelay", ui->delayDoubleSpinBox->value());
	settings.setValue("Xon", ui->xonInput->text());
	settings.setValue("Xoff", ui->xoffInput->text());
	settings.setValue("DeleteControlChars", ui->deleteControlChars->isChecked());
	settings.setValue("RemoveEmptyLines", ui->removeEmptyLines->isChecked());
	settings.setValue("RemoveBefore", ui->removeBefore->isChecked());
	settings.setValue("SendingStartDelay", ui->startDelaySpinBox->value());
	settings.setValue("ReceiveTimeoutTime", ui->receiveTimeoutSpinBox->value());
	settings.setValue("SendTimeoutTime", ui->sendTimeoutSpinBox->value());
	settings.setValue("AutoCloseTime", ui->autoCloseSpinBox->value());
	//settings.setValue("EndOfBlockLF", endOfBlockLF->isChecked());
	settings.setValue("RemoveSpaceEOB", ui->removeSpaceEOB->isChecked());
	settings.setValue("EobChar", ui->eobComboBox->currentText());
	settings.setValue("DataToLogFile", ui->logDataCheckBox->isChecked());
	settings.setValue("WaitForCTS", ui->waitForCtsCheckBox->isChecked());
	settings.setValue("WaitForXON", ui->waitForXonCheckBox->isChecked());
	settings.setValue("SendXONAtStart", ui->sendXonCheckBox->isChecked());
	settings.setValue("RemoveFromReceived", ui->removeLineEdit->text());

	settings.setValue("AutoSave", ui->autoSaveCheckBox->isChecked());
	settings.setValue("CreateLogFile", ui->logFileCheckBox->isChecked());
	settings.setValue("CreateBackup", ui->renameCheckBox->isChecked());
	settings.setValue("RemoveLetters", ui->removeLettersCheckBox->isChecked());
	settings.setValue("DetectFormFileName", ui->detectFormFileNameCheckBox->isChecked());
	settings.setValue("AppendExtension", ui->appendExtCheckBox->isChecked());
	settings.setValue("UseAsExtension", ui->useAsExtCheckBox->isChecked());
	settings.setValue("SplitPrograms", ui->splitProgramsCheckBox->isChecked());
	settings.setValue("SavePath", ui->savePathLineEdit->text());
	settings.setValue("SaveExt", ui->saveExtComboBox->currentText());

	settings.setValue("FileServer", ui->fileServerCheckBox->isChecked());
	settings.setValue("FileNameExpFSSelected", ui->fileNameExpFSComboBox->currentText());
	settings.setValue("FileNameExpASSelected", ui->fileNameExpASComboBox->currentText());
	settings.setValue("EndOfProgExpSelected", ui->endOfProgComboBox->currentText());
	settings.setValue("FileNameLowerCase", ui->fileNameLowerCaseCheckBox->isChecked());
	settings.setValue("CallerProg", ui->callerProgNameLineEdit->text());
	settings.setValue("SearchPath1", ui->searchPath1LineEdit->text());
	settings.setValue("SearchExt1", ui->searchExt1ComboBox->currentText());
	settings.setValue("SearchPath2", ui->searchPath2LineEdit->text());
	settings.setValue("SearchExt2", ui->searchExt2ComboBox->currentText());
	settings.setValue("SearchPath3", ui->searchPath3LineEdit->text());
	settings.setValue("SearchExt3", ui->searchExt3ComboBox->currentText());

	settings.endGroup();

	QStringList eList;
	eList.prepend(ui->fileNameExpFSComboBox->currentText());

	for (int i = 0; i <= ui->fileNameExpFSComboBox->count(); i++) {
		item = ui->fileNameExpFSComboBox->itemText(i);

		if (!item.isEmpty())
			if (!eList.contains(item)) {
				eList.prepend(item);
			}
	}

	eList.removeDuplicates();
	eList.sort();
	settings.setValue("FileNameExpFS", eList);

	eList.clear();
	eList.prepend(ui->fileNameExpASComboBox->currentText());

	for (int i = 0; i <= ui->fileNameExpASComboBox->count(); i++) {
		item = ui->fileNameExpASComboBox->itemText(i);

		if (!item.isEmpty())
			if (!eList.contains(item)) {
				eList.prepend(item);
			}
	}

	eList.removeDuplicates();
	eList.sort();
	settings.setValue("FileNameExpAS", eList);

	eList.clear();
	eList.prepend(ui->endOfProgComboBox->currentText());

	for (int i = 0; i <= ui->endOfProgComboBox->count(); i++) {
		item = ui->endOfProgComboBox->itemText(i);

		if (!item.isEmpty())
			if (!eList.contains(item)) {
				eList.prepend(item);
			}
	}

	eList.removeDuplicates();
	eList.sort();
	settings.setValue("EndOfProgExp", eList);

	eList.clear();
	eList.prepend(ui->eobComboBox->currentText());

	for (int i = 0; i <= ui->eobComboBox->count(); i++) {
		item = ui->eobComboBox->itemText(i);

		if (!item.isEmpty())
			if (!eList.contains(item)) {
				eList.prepend(item);
			}
	}

	eList.removeDuplicates();
	eList.sort();
	settings.setValue("EndOfBlockCodes", eList);

	settings.endGroup();

	ui->configNameBox->clear();
	ui->configNameBox->addItems(list);
	int id = ui->configNameBox->findText(curItem);

	if (id >= 0) {
		ui->configNameBox->setCurrentIndex(id);
	}
}

void GUI::SerialPortConfigDialog::changeSettings()
{
	int id;
	QString item, port;
	QStringList list;

	QSettings& settings = *Medium::instance().settings();

	QStringList extensions = settings.value("Extensions",
	                                        (QStringList() << "*.nc" << "*.cnc")).toStringList();
	extensions.removeDuplicates();
	extensions.replaceInStrings("*", "");
	ui->saveExtComboBox->insertItems(0, extensions);
	ui->searchExt1ComboBox->insertItems(0, extensions);
	ui->searchExt2ComboBox->insertItems(0, extensions);
	ui->searchExt3ComboBox->insertItems(0, extensions);

	settings.beginGroup("SerialPortConfigs");

	list.clear();
	list.append(settings.value("EndOfBlockCodes",
	                           (QStringList() << "LF" << "CR" << "CRLF" << "CRCRLF" << "LFCRCR" << "")).toStringList());
	list.removeDuplicates();
	ui->eobComboBox->insertItems(0, list);

	list.clear();
	list.append(settings.value("EndOfProgExp",
	                           (QStringList() << "(M30|M02|M2|M99)[\\n\\r \\t]{1,}%" << "")).toStringList());
	list.removeDuplicates();
	ui->endOfProgComboBox->insertItems(0, list);

	list.clear();
	list.append(settings.value("FileNameExpFS",
	                           (QStringList() << "\\([0-9]{4,4}\\)" << "\\([0-9]{4,4}\\.NC\\)" <<
	                            "\\((O|:){0,1}[0-9]{4,4}\\.(NC|CNC|AGC){1,1}\\)")).toStringList());
	list.removeDuplicates();
	ui->fileNameExpFSComboBox->insertItems(0, list);

	list.clear();
	list.append(settings.value("FileNameExpAS",
	                           (QStringList() << "\\([0-9]{4,4}\\)" << "\\([0-9]{4,4}\\.NC\\)" <<
	                            "\\((O|:){0,1}[0-9]{4,4}\\.(NC|CNC|AGC){1,1}\\)")).toStringList());
	list.removeDuplicates();
	ui->fileNameExpASComboBox->insertItems(0, list);

	settings.beginGroup(ui->configNameBox->currentText());

#ifdef Q_OS_WIN32
	port = "COM1";
#else
	port = "/dev/ttyUSB0";
#endif

	item = settings.value("PortName", port).toString();
	id = ui->portNameComboBox->findText(item);

	if (id == -1) {
		ui->portNameComboBox->addItems(QStringList(item));
		id = ui->portNameComboBox->findText(item);
	}

	ui->portNameComboBox->setCurrentIndex(id);
	portNameComboBoxIndexChanged(item);

	id = settings.value("BaudRate", QSerialPort::Baud9600).toInt();

	switch (id) {
	case 300:
		ui->b1CheckBox->setChecked(true);
		break;

	case 600:
		ui->b2CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud1200:
		ui->b3CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud2400:
		ui->b4CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud4800:
		ui->b5CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud9600:
		ui->b6CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud19200:
		ui->b7CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud38400:
		ui->b8CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud57600:
		ui->b9CheckBox->setChecked(true);
		break;

	case QSerialPort::Baud115200:
		ui->b11CheckBox->setChecked(true);
		break;
	}

	id = settings.value("DataBits", QSerialPort::Data8).toInt();

	switch (id) {
	case QSerialPort::Data5:
		ui->d1CheckBox->setChecked(true);
		break;

	case QSerialPort::Data6:
		ui->d2CheckBox->setChecked(true);
		break;

	case QSerialPort::Data7:
		ui->d3CheckBox->setChecked(true);
		break;

	case QSerialPort::Data8:
		ui->d4CheckBox->setChecked(true);
		break;
	}

	id = settings.value("StopBits", QSerialPort::TwoStop).toInt();

	switch (id) {
	case QSerialPort::OneStop:
		ui->s1CheckBox->setChecked(true);
		break;

	case QSerialPort::TwoStop:
		ui->s2CheckBox->setChecked(true);
		break;
	}

	id = settings.value("Parity", QSerialPort::NoParity).toInt();

	switch (id) {
	case QSerialPort::NoParity:
		ui->p1CheckBox->setChecked(true);
		break;

	case QSerialPort::OddParity:
		ui->p2CheckBox->setChecked(true);
		break;

	case QSerialPort::EvenParity:
		ui->p3CheckBox->setChecked(true);
		break;
	}

	id = settings.value("FlowControl", QSerialPort::HardwareControl).toInt();

	switch (id) {
	case QSerialPort::NoFlowControl:
		ui->f1CheckBox->setChecked(true);
		break;

	case QSerialPort::HardwareControl:
		ui->f2CheckBox->setChecked(true);
		break;

	case QSerialPort::SoftwareControl:
		ui->f3CheckBox->setChecked(true);
		break;
	}

	ui->eotInput->setText(settings.value("SendAtEnd", "").toString());
	ui->stInput->setText(settings.value("SendAtBegining", "").toString());
	ui->xonInput->setText(settings.value("Xon", "0").toString());
	ui->xoffInput->setText(settings.value("Xoff", "0").toString());
	ui->delayDoubleSpinBox->setValue(settings.value("LineDelay", 0).toDouble());
	ui->deleteControlChars->setChecked(settings.value("DeleteControlChars", true).toBool());
	ui->removeEmptyLines->setChecked(settings.value("RemoveEmptyLines", true).toBool());
	ui->removeBefore->setChecked(settings.value("RemoveBefore", false).toBool());
	ui->startDelaySpinBox->setValue(settings.value("SendingStartDelay", 0).toInt());
	ui->autoCloseSpinBox->setValue(settings.value("AutoCloseTime", 15).toInt());
	ui->sendTimeoutSpinBox->setValue(settings.value("SendTimeoutTime", 30).toInt());
	ui->receiveTimeoutSpinBox->setValue(settings.value("ReceiveTimeoutTime", 3).toInt());
	ui->removeSpaceEOB->setChecked(settings.value("RemoveSpaceEOB", false).toBool());
	ui->logFileCheckBox->setChecked(settings.value("CreateLogFile", true).toBool());
	ui->logDataCheckBox->setChecked(settings.value("DataToLogFile", false).toBool());
	ui->waitForCtsCheckBox->setChecked(settings.value("WaitForCTS", false).toBool());
	ui->waitForXonCheckBox->setChecked(settings.value("WaitForXON", false).toBool());
	ui->sendXonCheckBox->setChecked(settings.value("SendXONAtStart", false).toBool());
	ui->removeLineEdit->setText(settings.value("RemoveFromReceived", "").toString());

	id = ui->eobComboBox->findText(settings.value("EobChar", "CRLF").toString());

	if (id >= 0) {
		ui->eobComboBox->setCurrentIndex(id);
	}

	ui->autoSaveCheckBox->setChecked(settings.value("AutoSave", false).toBool());
	ui->renameCheckBox->setChecked(settings.value("CreateBackup", true).toBool());
	ui->removeLettersCheckBox->setChecked(settings.value("RemoveLetters", true).toBool());
	ui->detectFormFileNameCheckBox->setChecked(settings.value("DetectFormFileName", true).toBool());
	ui->appendExtCheckBox->setChecked(settings.value("AppendExtension", false).toBool());
	ui->useAsExtCheckBox->setChecked(settings.value("UseAsExtension", false).toBool());
	ui->splitProgramsCheckBox->setChecked(settings.value("SplitPrograms", true).toBool());
	ui->savePathLineEdit->setText(settings.value("SavePath", "").toString());
	id = ui->saveExtComboBox->findText(settings.value("SaveExt", ".nc").toString());

	if (id >= 0) {
		ui->saveExtComboBox->setCurrentIndex(id);
	}

	id = ui->endOfProgComboBox->findText(settings.value("EndOfProgExpSelected", "").toString());

	if (id >= 0) {
		ui->endOfProgComboBox->setCurrentIndex(id);
	}

	ui->fileServerCheckBox->setChecked(settings.value("FileServer", false).toBool());
	ui->callerProgNameLineEdit->setText(settings.value("CallerProg", "O5555").toString());
	ui->fileNameLowerCaseCheckBox->setChecked(settings.value("FileNameLowerCase", true).toBool());
	ui->reconnectSpinBox->setValue(settings.value("ReconnectTimeoutTime", 60).toInt());

	id = ui->fileNameExpFSComboBox->findText(settings.value("FileNameExpFSSelected", "").toString());

	if (id >= 0) {
		ui->fileNameExpFSComboBox->setCurrentIndex(id);
	}

	id = ui->fileNameExpASComboBox->findText(settings.value("FileNameExpASSelected", "").toString());

	if (id >= 0) {
		ui->fileNameExpASComboBox->setCurrentIndex(id);
	}

	ui->searchPath1LineEdit->setText(settings.value("SearchPath1", "").toString());
	id = ui->searchExt1ComboBox->findText(settings.value("SearchExt1", ".nc").toString());

	if (id >= 0) {
		ui->searchExt1ComboBox->setCurrentIndex(id);
	}

	ui->searchPath2LineEdit->setText(settings.value("SearchPath2", "").toString());
	id = ui->searchExt2ComboBox->findText(settings.value("SearchExt2", ".nc").toString());

	if (id >= 0) {
		ui->searchExt2ComboBox->setCurrentIndex(id);
	}

	ui->searchPath3LineEdit->setText(settings.value("SearchPath3", "").toString());
	id = ui->searchExt3ComboBox->findText(settings.value("SearchExt3", ".nc").toString());

	if (id >= 0) {
		ui->searchExt3ComboBox->setCurrentIndex(id);
	}

	settings.endGroup();
	settings.endGroup();

	autoSaveCheckBoxChanged((ui->autoSaveCheckBox->isChecked() ? Qt::Checked : Qt::Unchecked));
	fileServerCheckBoxChanged((ui->fileServerCheckBox->isChecked() ? Qt::Checked : Qt::Unchecked));
}

void GUI::SerialPortConfigDialog::loadSettings()
{
	int id;
	QStringList list;

	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("SerialPortConfigs");

	foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
		list.append(info.portName());
	}

	list.removeDuplicates();
	list.sort();
	ui->portNameComboBox->clear();
	ui->portNameComboBox->addItems(list);

	ui->configNameBox->clear();
	list = settings.value("SettingsList", tr("Default")).toStringList();
	list.sort();
	ui->configNameBox->addItems(list);
	//item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
	id = ui->configNameBox->findText(configName);
	ui->configNameBox->setCurrentIndex(id);
	settings.endGroup();

	changeSettings();
}

void GUI::SerialPortConfigDialog::portNameComboBoxIndexChanged(QString name)
{
	QString description;
	QString manufacturer;
	QString serialNumber;

	QSerialPortInfo* info = new QSerialPortInfo(name);

	description = info->description();
	manufacturer = info->manufacturer();
	serialNumber = info->serialNumber();

	ui->portInfoListWidget->clear();
	//ui->portInfoListWidget->addItem(tr("Port name:          %1").arg(info->portName()));
	ui->portInfoListWidget->addItem(tr("Description       : %1").arg((!description.isEmpty() ?
	                                description : blankString)));
	ui->portInfoListWidget->addItem(tr("Manufacturer      : %1").arg((!manufacturer.isEmpty() ?
	                                manufacturer : blankString)));
	ui->portInfoListWidget->addItem(tr("SerialNumber      : %1").arg((!serialNumber.isEmpty() ?
	                                serialNumber : blankString)));
	ui->portInfoListWidget->addItem(tr("System location   : %1").arg(info->systemLocation()));
	ui->portInfoListWidget->addItem(tr("Vendor identifier : %1").arg((info->vendorIdentifier() ?
	                                QString::number(info->vendorIdentifier(), 16) : blankString)));
	ui->portInfoListWidget->addItem(tr("Product identifier: %1").arg((info->productIdentifier() ?
	                                QString::number(info->productIdentifier(), 16) : blankString)));
}

void GUI::SerialPortConfigDialog::deleteButtonClicked()
{
	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("SerialPortConfigs");
	settings.remove(ui->configNameBox->currentText());
	settings.endGroup();

	int id = ui->configNameBox->findText(ui->configNameBox->currentText());
	ui->configNameBox->removeItem(id);
}

void GUI::SerialPortConfigDialog::closeButtonClicked()
{
	QSettings& settings = *Medium::instance().settings();
	settings.beginGroup("SerialPortConfigs");
	settings.setValue("CurrentSerialPortSettings", ui->configNameBox->currentText());
	settings.endGroup();

	close();
}

void GUI::SerialPortConfigDialog::saveCloseButtonClicked()
{
	saveButtonClicked();
	closeButtonClicked();
	accept();
}

QString GUI::SerialPortConfigDialog::browseForDir(QString dir, QString windowTitle)
{
	QDir directory(dir);

	if ((!directory.exists()) || (!directory.isReadable()) || (dir.isEmpty())) {
		directory.setPath(QDir::homePath());
	}

	dir = QFileDialog::getExistingDirectory(
	          this,
	          windowTitle,
	          directory.canonicalPath(),
	          QFileDialog::ShowDirsOnly);

	if (!dir.isEmpty()) {
		directory.setPath(dir);
	}

	return directory.canonicalPath();
}

void GUI::SerialPortConfigDialog::browse1ButtonClicked()
{
	ui->savePathLineEdit->setText(browseForDir(ui->savePathLineEdit->text(), tr("Select search path")));
}

void GUI::SerialPortConfigDialog::browse2ButtonClicked()
{
	ui->searchPath1LineEdit->setText(browseForDir(ui->searchPath1LineEdit->text(),
	                                 tr("Select search path")));
}

void GUI::SerialPortConfigDialog::browse3ButtonClicked()
{
	ui->searchPath2LineEdit->setText(browseForDir(ui->searchPath2LineEdit->text(),
	                                 tr("Select search path")));
}

void GUI::SerialPortConfigDialog::autoSaveCheckBoxChanged(int state)
{
	ui->savePathLineEdit->setEnabled(state == Qt::Checked);
	ui->saveExtComboBox->setEnabled(state == Qt::Checked);
	ui->browse1PushButton->setEnabled(state == Qt::Checked);
	ui->renameCheckBox->setEnabled(state == Qt::Checked);
	ui->removeLettersCheckBox->setEnabled(state == Qt::Checked);
	ui->detectFormFileNameCheckBox->setEnabled(state == Qt::Checked);
	ui->appendExtCheckBox->setEnabled(state == Qt::Checked);
	ui->useAsExtCheckBox->setEnabled(state == Qt::Checked);
	ui->splitProgramsCheckBox->setEnabled(state == Qt::Checked);
	ui->endOfProgComboBox->setEnabled(state == Qt::Checked);
	ui->addEobButton->setEnabled(state == Qt::Checked);
	ui->deleteEobButton->setEnabled(state == Qt::Checked);
	ui->tabWidget->setTabEnabled(4, state == Qt::Checked);

	if (ui->fileServerCheckBox->isChecked() && !(state == Qt::Checked)) {
		ui->fileServerCheckBox->setChecked(false);
	}

	flowCtlGroupReleased();
}

void GUI::SerialPortConfigDialog::browse4ButtonClicked()
{
	ui->searchPath3LineEdit->setText(browseForDir(ui->searchPath3LineEdit->text(),
	                                 tr("Select search path")));
}

void GUI::SerialPortConfigDialog::appendExtCheckBoxChanged(int state)
{
	if (state == Qt::Checked) {
		ui->useAsExtCheckBox->setChecked(false);
	}
}

void GUI::SerialPortConfigDialog::useAsExtCheckBoxChanged(int state)
{
	if (state == Qt::Checked) {
		ui->appendExtCheckBox->setChecked(false);
	}

}

void GUI::SerialPortConfigDialog::fileServerCheckBoxChanged(int state)
{
	ui->searchPath1LineEdit->setEnabled(state == Qt::Checked);
	ui->searchPath2LineEdit->setEnabled(state == Qt::Checked);
	ui->searchPath3LineEdit->setEnabled(state == Qt::Checked);
	ui->searchExt1ComboBox->setEnabled(state == Qt::Checked);
	ui->searchExt2ComboBox->setEnabled(state == Qt::Checked);
	ui->searchExt3ComboBox->setEnabled(state == Qt::Checked);
	ui->browse2PushButton->setEnabled(state == Qt::Checked);
	ui->browse3PushButton->setEnabled(state == Qt::Checked);
	ui->browse4PushButton->setEnabled(state == Qt::Checked);
	ui->fileNameExpFSComboBox->setEnabled(state == Qt::Checked);
	ui->removeButton->setEnabled(state == Qt::Checked);
	ui->addButton->setEnabled(state == Qt::Checked);
	ui->callerProgNameLineEdit->setEnabled(state == Qt::Checked);
	ui->fileNameLowerCaseCheckBox->setEnabled(state == Qt::Checked);
	flowCtlGroupReleased();
}

void GUI::SerialPortConfigDialog::addButtonClicked()
{
	QString text = ui->fileNameExpFSComboBox->currentText();

	if (!text.isEmpty()) {
		ui->fileNameExpFSComboBox->insertItem(0, text);
	}
}

void GUI::SerialPortConfigDialog::removeButtonClicked()
{
	ui->fileNameExpFSComboBox->removeItem(ui->fileNameExpFSComboBox->currentIndex());
}

void GUI::SerialPortConfigDialog::addEobButtonClicked()
{
	QString text = ui->endOfProgComboBox->currentText();

	if (!text.isEmpty()) {
		ui->endOfProgComboBox->insertItem(0, text);
	}
}

void GUI::SerialPortConfigDialog::deleteEobButtonClicked()
{
	ui->endOfProgComboBox->removeItem(ui->endOfProgComboBox->currentIndex());
}

void GUI::SerialPortConfigDialog::addEobCharButtonClicked()
{
	QString text = ui->eobComboBox->currentText();

	if (!text.isEmpty()) {
		ui->eobComboBox->insertItem(0, text);
	}
}

void GUI::SerialPortConfigDialog::deleteEobCharButtonClicked()
{
	ui->eobComboBox->removeItem(ui->eobComboBox->currentIndex());
}

void GUI::SerialPortConfigDialog::eobComboBoxEditTextChanged(const QString text)
{
	ui->eobComboBox->setCurrentText(text.toUpper());
}

void GUI::SerialPortConfigDialog::addFileNameButtonClicked()
{
	QString text = ui->fileNameExpASComboBox->currentText();

	if (!text.isEmpty()) {
		ui->fileNameExpASComboBox->insertItem(0, text);
	}
}

void GUI::SerialPortConfigDialog::removeFileNameButtonClicked()
{
	ui->fileNameExpASComboBox->removeItem(ui->fileNameExpASComboBox->currentIndex());
}

void GUI::SerialPortConfigDialog::helpButtonClicked()
{
	// TODO: change a path to the file
	QDesktopServices::openUrl(QUrl(QString("file:///%1/SerialTransmission_Help.html").arg(
	                                   QCoreApplication::applicationDirPath()), QUrl::TolerantMode));
	//    SerialPortCfgHelpDialog helpDialog;
	//    helpDialog.exec();
}

void GUI::SerialPortConfigDialog::readPath1Changed(const QString text)
{
	QPalette palette;

	if (text.isEmpty()) {
		return;
	}

	palette.setColor(ui->searchPath1LineEdit->foregroundRole(), Qt::red);

	QDir dir;
	dir.setPath(text);

	if (dir.exists()) {
		ui->searchPath1LineEdit->setPalette(QPalette());
	} else {
		ui->searchPath1LineEdit->setPalette(palette);
	}
}

void GUI::SerialPortConfigDialog::readPath2Changed(const QString text)
{
	QPalette palette;

	if (text.isEmpty()) {
		return;
	}

	palette.setColor(ui->searchPath2LineEdit->foregroundRole(), Qt::red);

	QDir dir;
	dir.setPath(text);

	if (dir.exists()) {
		ui->searchPath2LineEdit->setPalette(QPalette());
	} else {
		ui->searchPath2LineEdit->setPalette(palette);
	}
}

void GUI::SerialPortConfigDialog::readPath3Changed(const QString text)
{
	QPalette palette;

	if (text.isEmpty()) {
		return;
	}

	palette.setColor(ui->searchPath3LineEdit->foregroundRole(), Qt::red);

	QDir dir;
	dir.setPath(text);

	if (dir.exists()) {
		ui->searchPath3LineEdit->setPalette(QPalette());
	} else {
		ui->searchPath3LineEdit->setPalette(palette);
	}
}
