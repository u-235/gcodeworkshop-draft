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

#include <QApplication>     // for QApplication, qApp
#include <QBrush>           // for QBrush
#include <QChar>            // for QChar
#include <QComboBox>        // for QComboBox
#include <QIODevice>        // for QIODevice
#include <QIcon>            // for QIcon
#include <QLabel>           // for QLabel
#include <QLineEdit>        // for QLineEdit
#include <QPlainTextEdit>   // for QPlainTextEdit
#include <QPushButton>      // for QPushButton
#include <QScrollBar>       // for QScrollBar
#include <QSerialPort>      // for QSerialPort
#include <QSettings>        // for QSettings
#include <QString>          // for QString, QCharRef
#include <QStringList>      // for QStringList
#include <QTextCharFormat>  // for QTextCharFormat
#include <QTextCursor>      // for QTextCursor
#include <QTimer>           // for QTimer
#include <QToolButton>      // for QToolButton
#include <QVariant>         // for QVariant
#include <Qt>               // for GlobalColor, FocusReason, WidgetAttribute, WindowFlags
#include <QtGlobal>         // for QFlags, Q_UNUSED, Q_OS_WIN32

class QWidget;

#include <utils/medium.h>   // for Medium

#include <gui/serialport/serialportconfigdialog.h>  //  for SerialPortConfigDialog
#include <gui/serialport/serialporttestdialog.h>

#include "ui_serialporttestdialog.h"    // for Ui::SerialPortTestDialog


GUI::SerialPortTestDialog::SerialPortTestDialog(QWidget* parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::SerialPortTestDialog())
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Serial transmission test"));

	comPort = nullptr;

	connect(ui->closeButton, SIGNAL(clicked()), SLOT(closeButtonClicked()));

	connect(ui->clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));
	connect(ui->configButton, SIGNAL(clicked()), SLOT(configButtonClicked()));

	connect(ui->connectButton, SIGNAL(toggled(bool)), SLOT(connectButtonToggled(bool)));

	connect(ui->setRtsButton, SIGNAL(clicked()), SLOT(setRtsButtonClicked()));
	connect(ui->setDtrButton, SIGNAL(clicked()), SLOT(setDtrButtonClicked()));

	loadSerialConfignames();
	changeSettings();
	connect(ui->configBox, SIGNAL(currentIndexChanged(int)), SLOT(changeSettings()));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateLeds()));

	connect(ui->sendLineEdit, SIGNAL(returnPressed()), SLOT(sendTextEditChanged()));

	connect(ui->setXonButton, SIGNAL(clicked()), SLOT(setXonButtonClicked()));
	connect(ui->setXoffButton, SIGNAL(clicked()), SLOT(setXoffButtonClicked()));


	connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(textEditScroll(int)));
	connect(ui->hexTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)),
	        SLOT(hexTextEditScroll(int)));
}

GUI::SerialPortTestDialog::~SerialPortTestDialog()
{
	delete ui;
}

void GUI::SerialPortTestDialog::textEditScroll(int pos)
{
	Q_UNUSED(pos);
	ui->hexTextEdit->verticalScrollBar()->setSliderPosition(
	    ui->textEdit->verticalScrollBar()->sliderPosition());
}

void GUI::SerialPortTestDialog::hexTextEditScroll(int pos)
{
	Q_UNUSED(pos);
	ui->textEdit->verticalScrollBar()->setSliderPosition(
	    ui->hexTextEdit->verticalScrollBar()->sliderPosition());
}

void GUI::SerialPortTestDialog::sendTextEditChanged()
{
	QString tx, ty;
	int i;

	tx = ui->sendLineEdit->text();
	ui->sendLineEdit->clear();

	tx.append("\r\n");

	sendText(tx);

	QTextCursor cr = ui->textEdit->textCursor();
	QTextCharFormat format = cr.charFormat();
	format.setForeground(Qt::black);
	cr.setCharFormat(format);
	cr.insertText(tx);
	ui->textEdit->setTextCursor(cr);

	cr = ui->hexTextEdit->textCursor();
	format = cr.charFormat();
	format.setForeground(Qt::black);
	cr.setCharFormat(format);

	for (i = 0; i < tx.size(); i++) {
		ty = QString("%1 ").arg((int)tx.at(i).toLatin1(), 2, 16, (QChar)'0');

		cr.insertText(ty);
	}

	ui->hexTextEdit->setTextCursor(cr);
	ui->hexTextEdit->insertPlainText("\n");

	ui->textEdit->ensureCursorVisible();
	ui->hexTextEdit->ensureCursorVisible();
}

void GUI::SerialPortTestDialog::closeButtonClicked()
{
	stop = true;
	qApp->processEvents();

	if (comPort != nullptr) {
		//comPort->close();
		//delete(comPort);
	}

	close();
}

void GUI::SerialPortTestDialog::clearButtonClicked()
{
	ui->textEdit->clear();
	ui->hexTextEdit->clear();
	ui->sendLineEdit->clear();
}

void GUI::SerialPortTestDialog::connectButtonToggled(bool tg)
{
	if (tg) {

		ui->connectButton->setIcon(QIcon(":/images/connect_established.png"));
		ui->connectButton->setText(tr("&Disconnect"));

		ui->textEdit->clear();
		ui->hexTextEdit->clear();
		ui->sendLineEdit->clear();

		count = 0;

		if (comPort != nullptr) {
			comPort->reset();
			comPort->close();
			delete comPort;
		}

		comPort = new QSerialPort(portName);
		comPort->clearError();
		comPort->setBaudRate(portSettings.BaudRate);
		comPort->setDataBits(portSettings.DataBits);
		comPort->setParity(portSettings.Parity);
		comPort->setStopBits(portSettings.StopBits);
		comPort->setFlowControl(portSettings.FlowControl);

		if (!comPort->open(QIODevice::ReadWrite)) {
			showError(comPort->error());
			delete comPort;
			comPort = nullptr;
			ui->connectButton->setChecked(false);
			return;
		}

		comPort->clear(QSerialPort::AllDirections);

		//      if(portSettings.FlowControl == FLOW_XONXOFF)
		//      {
		//         comPort->putChar(portSettings.Xon);
		//      }

		ui->setDtrButton->setEnabled(true);
		ui->setDtrButton->setChecked(false);
		ui->setRtsButton->setEnabled(true);
		ui->setRtsButton->setChecked(false);

		ui->configBox->setEnabled(false);
		ui->configButton->setEnabled(false);

		bool en = portSettings.FlowControl == QSerialPort::SoftwareControl;
		ui->setXonButton->setEnabled(en);
		ui->setXoffButton->setEnabled(en);

		ui->sendLineEdit->setReadOnly(false);
		ui->sendLineEdit->setFocus(Qt::MouseFocusReason);

		showError(comPort->error());

		stop = false;
		timer->start(20);

	} else {
		timer->stop();
		//qApp->processEvents();
		ui->connectButton->setIcon(QIcon(":/images/connect_no.png"));
		ui->connectButton->setText(tr("&Connect"));

		stop = true;
		qApp->processEvents();
		ui->setDtrButton->setEnabled(false);
		ui->setRtsButton->setEnabled(false);
		ui->setXonButton->setEnabled(false);
		ui->setXoffButton->setEnabled(false);

		ui->ctsLabel->setEnabled(false);
		ui->dsrLabel->setEnabled(false);
		ui->dcdLabel->setEnabled(false);
		ui->rtsLabel->setEnabled(false);
		ui->dtrLabel->setEnabled(false);

		ui->connectButton->setChecked(false);

		ui->configBox->setEnabled(true);
		ui->configButton->setEnabled(true);

		ui->sendLineEdit->setReadOnly(true);
	}
}

void GUI::SerialPortTestDialog::setXonButtonClicked()
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->putChar(portSettings.Xon);
	}
}

void GUI::SerialPortTestDialog::setXoffButtonClicked()
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->putChar(portSettings.Xoff);
	}
}

void GUI::SerialPortTestDialog::setDtrButtonClicked()
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->setDataTerminalReady(ui->setDtrButton->isChecked());
	}
}

void GUI::SerialPortTestDialog::setRtsButtonClicked()
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->setRequestToSend(ui->setRtsButton->isChecked());
	}
}

void GUI::SerialPortTestDialog::updateLeds()
{
	QSerialPort::PinoutSignals status;
	bool ok;
	QString tx;
	char ch;
	int i;
	QTextCursor cr;
	QTextCharFormat format;

	timer->stop();

	if (comPort == nullptr || !comPort->isOpen()) {
		return;
	}

	while (!stop) {
		status = comPort->pinoutSignals();

		ui->ctsLabel->setEnabled(status & QSerialPort::ClearToSendSignal);
		ui->dsrLabel->setEnabled(status & QSerialPort::DataSetReadySignal);
		ui->dcdLabel->setEnabled(status & QSerialPort::DataCarrierDetectSignal);
		ui->rtsLabel->setEnabled(status & QSerialPort::RequestToSendSignal);
		ui->setRtsButton->setChecked(status & QSerialPort::RequestToSendSignal);
		ui->dtrLabel->setEnabled(status & QSerialPort::DataTerminalReadySignal);
		ui->setDtrButton->setChecked(status & QSerialPort::DataTerminalReadySignal);


		i = comPort->bytesAvailable();
		qApp->processEvents();

		if (i > 0) {
			ok = comPort->getChar(&ch);

			if (!ok) {
				showError(comPort->error());
				return;
			}

			count++;
			ui->errorLabel->setText(tr("Received: %1 bytes.").arg(count));
			tx = ch;

			qApp->processEvents();

			if (ch != '\r') {
				cr = ui->textEdit->textCursor();
				format = cr.charFormat();
				format.setForeground(Qt::blue);
				cr.setCharFormat(format);
				cr.insertText(tx);
				ui->textEdit->setTextCursor(cr);
			}

			tx = QString("%1 ").arg((int)ch, 2, 16, (QChar)'0');
			cr = ui->hexTextEdit->textCursor();
			format = cr.charFormat();
			format.setForeground(Qt::blue);
			cr.setCharFormat(format);
			cr.insertText(tx);
			ui->hexTextEdit->setTextCursor(cr);

			if (ch == '\n') {
				ui->hexTextEdit->insertPlainText("\n");
			}

			ui->textEdit->ensureCursorVisible();
			ui->hexTextEdit->ensureCursorVisible();
		} else if (i < 0) {
			showError(comPort->error());
		}
	}

	comPort->close();
}

void GUI::SerialPortTestDialog::configButtonClicked()
{
	SerialPortConfigDialog* serialConfigDialog = new SerialPortConfigDialog(this,
	        ui->configBox->currentText());

	if (serialConfigDialog->exec() == QDialog::Accepted) {
		changeSettings();
	}
}

void GUI::SerialPortTestDialog::changeSettings()
{
	QString port;
	bool ok;

	QSettings& settings = *Medium::instance().settings();

	settings.beginGroup("SerialPortConfigs");

#ifdef Q_OS_WIN32
	port = "COM1";
#else
	port = "/dev/ttyS0";

#endif

	settings.beginGroup(ui->configBox->currentText());

	portName = settings.value("PortName", port).toString();

	portSettings.BaudRate = (QSerialPort::BaudRate) settings.value("BaudRate",
	                        QSerialPort::Baud9600).toInt();
	portSettings.DataBits = (QSerialPort::DataBits) settings.value("DataBits",
	                        QSerialPort::Data8).toInt();
	portSettings.StopBits = (QSerialPort::StopBits) settings.value("StopBits",
	                        QSerialPort::TwoStop).toInt();
	portSettings.Parity = (QSerialPort::Parity) settings.value("Parity",
	                      QSerialPort::NoParity).toInt();
	portSettings.FlowControl = (QSerialPort::FlowControl) settings.value("FlowControl",
	                           QSerialPort::HardwareControl).toInt();
	portSettings.Xon = settings.value("Xon", "17").toString().toInt(&ok, 10);
	portSettings.Xoff = settings.value("Xoff", "19").toString().toInt(&ok, 10);
	sendAtEnd = settings.value("SendAtEnd", "").toString();
	sendAtBegining = settings.value("SendAtBegining", "").toString();
	lineDelay = settings.value("LineDelay", 0).toDouble();

	settings.endGroup();
	settings.endGroup();
}

void GUI::SerialPortTestDialog::loadSerialConfignames()
{
	int id;
	QStringList list;
	QString item;

	QSettings& settings =  *Medium::instance().settings();

	settings.beginGroup("SerialPortConfigs");

	ui->configBox->clear();
	list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
	list.sort();
	ui->configBox->addItems(list);
	item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
	id = ui->configBox->findText(item);
	ui->configBox->setCurrentIndex(id);
	settings.endGroup();
}

void GUI::SerialPortTestDialog::showError(int error)
{
	switch (error) {
	case QSerialPort::NoError:
		ui->errorLabel->setText("No Error has occurred");
		break;

	case QSerialPort::DeviceNotFoundError:
		ui->errorLabel->setText("Attempting to open an non-existing device");
		break;

	case QSerialPort::PermissionError:
		ui->errorLabel->setText("Attempting to open an already opened device by another process or user not having enough permission and credentials to open");
		break;

	case QSerialPort::OpenError:
		ui->errorLabel->setText("Attempting to open an already opened device in this object");
		break;

	case QSerialPort::WriteError:
		ui->errorLabel->setText("An I/O error occurred while writing the data");
		break;

	case QSerialPort::ReadError:
		ui->errorLabel->setText("An I/O error occurred while reading the data");
		break;

	case QSerialPort::ResourceError:
		ui->errorLabel->setText("An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system");
		break;

	case QSerialPort::UnsupportedOperationError:
		ui->errorLabel->setText("Receive buffer overflow");
		break;

	case QSerialPort::UnknownError:
		ui->errorLabel->setText("An unidentified error occurred");
		break;

	case QSerialPort::TimeoutError:
		ui->errorLabel->setText("Transmit buffer overflow");
		break;

	case QSerialPort::NotOpenError:
		ui->errorLabel->setText("An operation is executed that can only be successfully performed if the device is open");
		break;

	default:
		ui->errorLabel->setText("Unknown error");
	}
}

void GUI::SerialPortTestDialog::sendText(QString tx)
{
	int i;
	bool xoffReceived;
	char controlChar;
	int bytesToWrite;

	if (comPort == nullptr) {
		return;
	}

	if (comPort->isOpen()) {
		tx.prepend(sendAtBegining);
		tx.append(sendAtEnd);

		if (!tx.contains("\r\n")) {
			tx.replace("\n", "\r\n");
		}

		ui->errorLabel->setText(tr("Waiting..."));
		qApp->processEvents();

		i = 0;
		xoffReceived = true;

		while (i < tx.size()) {
			if (xoffReceived) {
				ui->errorLabel->setText(tr("Waiting for a signal readiness..."));
			}

			qApp->processEvents();

			if (stop) {
				break;
			}

			if (portSettings.FlowControl == QSerialPort::SoftwareControl) {
				controlChar = 0;

				if (comPort->bytesAvailable() > 0) {
					comPort->getChar(&controlChar);
				}

				if (controlChar == portSettings.Xoff) {
					xoffReceived = true;
				}

				if (controlChar == portSettings.Xon) {
					xoffReceived = false;
				}

				//ui->setXoffButton->setChecked(xoffReceived);
				//ui->setXonButton->setChecked(!xoffReceived);
			} else {
				xoffReceived = false;
			}

			bytesToWrite = comPort->bytesToWrite();

			if ((bytesToWrite == 0) && (!xoffReceived)) {
				if (!comPort->putChar(tx[i].toLatin1())) {
					showError(comPort->error());
				}

				ui->errorLabel->setText(tr("Sending byte %1 of %2").arg(i + 1).arg(tx.size()));
				qApp->processEvents();

				if (lineDelay > 0) {
					if (tx[i].toLatin1() == '\n') {
						readyCont = false;
						QTimer::singleShot(int(lineDelay * 1000), this, SLOT(lineDelaySlot()));

						while (!readyCont) {
							qApp->processEvents();
						}
					}
				}

				i++;
			}
		}
	}
}

void GUI::SerialPortTestDialog::lineDelaySlot()
{
	readyCont = true;
}

//TransProgressDialog::TransProgressDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
//{
//   setupUi(this);
//   //setWindowTitle(tr("Serial transmission"));

//   comPort = nullptr;

//   canceled = true;

//   connect(cancelButton, SIGNAL(clicked()), SLOT(cancelButtonClicked()));
//   connect(this, SIGNAL(rejected()), SLOT(cancelButtonClicked()));

//   timer = new QTimer(this);
//   connect(timer, SIGNAL(timeout()), this, SLOT(updateLeds()));
//}

//TransProgressDialog::~TransProgressDialog()
//{
//   timer->stop();
//}

//void TransProgressDialog::closeEvent(QCloseEvent *event)
//{
//   timer->stop();
//   canceled = true;
//   event->accept();
//}

//void TransProgressDialog::cancelButtonClicked()
//{
//   timer->stop();
//   canceled = true;
//   close();
//}

//bool TransProgressDialog::wasCanceled()
//{
//   return canceled;
//}

//void TransProgressDialog::setLabelText(const QString text)
//{
//   label->setText(text);
//}

//void TransProgressDialog::setValue(int val)
//{
//   progressBar->setValue(val);
//}

//void TransProgressDialog::setRange(int min, int max)
//{
//   if(max == 0)
//   {
//      progressBar->hide();
//      cancelButton->setText(tr("&Close"));
//      cancelButton->setIcon(QIcon(":/images/window-close.png"));
//   }
//   else
//      progressBar->setRange(min, max);

//   progressBar->setValue(0);
//}

//void TransProgressDialog::open(QSerialPort *port)
//{
//   if(port != nullptr)
//   {
//      canceled = false;
//      show();
//      comPort = port;
//      timer->start(20);
//   }
//}

//void TransProgressDialog::updateLeds()
//{
//   timer->stop();
//   if(comPort == nullptr)
//      return;

//   if(!comPort->isOpen())
//      return;

//   QSerialPort::PinoutSignals status = comPort->pinoutSignals();
//   ui->ctsLabel->setEnabled(status & QSerialPort::ClearToSendSignal);
//   ui->dsrLabel->setEnabled(status & QSerialPort::DataSetReadySignal);
//   ui->dcdLabel->setEnabled(status & QSerialPort::DataCarrierDetectSignal);
//   ui->rtsLabel->setEnabled(status & QSerialPort::RequestToSendSignal);
//   ui->dtrLabel->setEnabled(status & QSerialPort::DataTerminalReadySignal);

//   timer->start();
//}
