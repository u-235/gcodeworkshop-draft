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

#ifndef GUI_SERIALTRANSMISSIONDIALOG_H
#define GUI_SERIALTRANSMISSIONDIALOG_H

#include <QByteArray>   // for QByteArray
#include <QDialog>      // for QDialog
#include <QObject>      // for slots, Q_OBJECT
#include <QSerialPort>  // for QSerialPort
#include <QString>      // for QString
#include <QStringList>  // for QStringList
#include <Qt>           // for WindowType, WindowFlags
#include <QtGlobal>     // for qint64

class QCloseEvent;
class QTimer;
class QWidget;

#include <serialportsettings.h>             // for SerialPortSettings

namespace Ui {
class SerialTransmissionDialog;
}

namespace GUI {
class SerialTransmissionDialog : public QDialog
{
	Q_OBJECT

public:
	SerialTransmissionDialog(QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog,
	                         bool mode = false);
	~SerialTransmissionDialog();

	QStringList receiveData(const QString& configName);
	void sendData(const QString& dataToSend, const QString& configName);
	void startFileServer(QString configName);
	QString configName();
	QString savePath();
	QStringList readPaths();

public slots:
	void loadTranslations();
	void loadIcons();

	bool wasCanceled();
	void portReset();

protected:
	void closeEvent(QCloseEvent* event);

private slots:
	void cancelButtonClicked();
	void updateStatus();
	void sendStartDelayTimeout();
	void stopButtonClicked();
	void autoCloseTimerTimeout();
	void showSerialPortError(QSerialPort::SerialPortError error);
	void serialPortBytesWritten(qint64 bytes);
	void serialPortReadyRead();
	void serialPortRequestToSend(bool set);
	void lineDelaySlot();
	void fileServerProcessData();
	void fileServerBytesWritten(qint64 bytes);
	void sendTimeoutTimerTimeout();
	void receiveTimeoutTimerTimeout();
	void reset(bool re);
	void reconnectTimerTimeout();

private:
	void setRange(int min, int max);
	void setValue(int val);
	void setLabelText(const QString text, bool multiline = false, bool writeToLog = false);
	void loadConfig(QString configName);
	QStringList guessFileName(const QString& text);
	QString saveDataToFile(const QString& text);
	void writeLog(QString msg, QString timeStamp = "");
	QStringList processReceivedData();
	void prepareDataBeforeSending(const QString& data);
	QString procesSpecialCharacters(const QString& text, const QString& fileData);
	void resetTransmission(bool portRestart = false);

	Ui::SerialTransmissionDialog* ui;
	QSerialPort serialPort;
	bool canceled;
	SerialPortSettings portSettings;
	QByteArray serialPortReadBuffer;
	// FIXME: serialPortWriteBuffer must be QList<QBiteArray>
	QStringList serialPortWriteBuffer;
	qint64 bytesWritten;
	qint64 noOfBytes;
	QStringList::iterator writeBufferIterator;
	bool xoffReceived;
	bool prevXoffReceived;
	unsigned int autoCloseCountner;
	unsigned int sendStartDelayCountner;
	unsigned int sendTimeoutCountner;
	unsigned int receiveTimeoutCountner;
	bool stop;
	bool serverMode;
	bool sending;
	QTimer* sendStartDelayTimer;
	QTimer* updateStatusTimer;
	QTimer* autoCloseTimer;
	QTimer* sendTimeoutTimer;
	QTimer* receiveTimeoutTimer;
	QTimer* reconnectTimer;
};
} // namespace GUI

#endif // GUI_SERIALTRANSMISSIONDIALOG_H
