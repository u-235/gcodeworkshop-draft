/*
 *  Copyright (C) 2026 Nick Egorrov, nicegorov@yandex.ru
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

#include <QSettings>    // for QSettings
#include <QVariant>     // for QVariant
#include <QtDebug>      // for QDebug
#include <QtGlobal>     // for qWarning

#include <serialportsettings.h>


#define CFG_KEY_APPEND_EXTENSION        "AppendExtension"
#define CFG_KEY_AUTO_CLOSE_TIME         "AutoCloseTime"
#define CFG_KEY_AUTO_SAVE               "AutoSave"
#define CFG_KEY_BAUD_RATE               "BaudRate"
#define CFG_KEY_CALLER_PROG             "CallerProg"
#define CFG_KEY_CREATE_BACKUP           "CreateBackup"
#define CFG_KEY_CREATE_LOG_FILE         "CreateLogFile"
#define CFG_KEY_DATA_BITS               "DataBits"
#define CFG_KEY_DETECT_FILE_NAME        "DetectFormFileName"
#define CFG_KEY_END_OF_BLOCK_CHAR       "EobChar"
#define CFG_KEY_END_OF_PROG_CHAR        "EndOfProgExpSelected"
#define CFG_KEY_FILE_NAME_EXP_AS        "FileNameExpASSelected"
#define CFG_KEY_FILE_NAME_EXP_FS        "FileNameExpFSSelected"
#define CFG_KEY_FILE_NAME_LOWER_CASE    "FileNameLowerCase"
#define CFG_KEY_FILE_SERVER             "FileServer"
#define CFG_KEY_FLOW_CONTROL            "FlowControl"
#define CFG_KEY_LINE_DELAY              "LineDelay"
#define CFG_KEY_LOG_TO_FILE             "DataToLogFile"
#define CFG_KEY_PARITY                  "Parity"
#define CFG_KEY_PORT_NAME               "PortName"
#define CFG_KEY_RECEIVE_TIMEOUT         "ReceiveTimeoutTime"
#define CFG_KEY_RECONNECT_TIMEOUT       "ReconnectTimeoutTime"
#define CFG_KEY_REMOVE_BEFORE           "RemoveBefore"
#define CFG_KEY_REMOVE_CONTROL_CHARS    "DeleteControlChars"
#define CFG_KEY_REMOVE_EMPTY_LINES      "RemoveEmptyLines"
#define CFG_KEY_REMOVE_FROM_RECEIVED    "RemoveFromReceived"
#define CFG_KEY_REMOVE_LETTERS          "RemoveLetters"
#define CFG_KEY_REMOVE_SPACE_EOB        "RemoveSpaceEOB"
#define CFG_KEY_SAVE_EXT                "SaveExt"
#define CFG_KEY_SAVE_PATH               "SavePath"
#define CFG_KEY_SEARCH_EXT_1            "SearchExt1"
#define CFG_KEY_SEARCH_EXT_2            "SearchExt2"
#define CFG_KEY_SEARCH_EXT_3            "SearchExt3"
#define CFG_KEY_SEARCH_PATH_1           "SearchPath1"
#define CFG_KEY_SEARCH_PATH_2           "SearchPath2"
#define CFG_KEY_SEARCH_PATH_3           "SearchPath3"
#define CFG_KEY_SENDING_START_DELAY     "SendingStartDelay"
#define CFG_KEY_SEND_AT_BEGINING        "SendAtBegining"
#define CFG_KEY_SEND_AT_END             "SendAtEnd"
#define CFG_KEY_SEND_TIMEOUT            "SendTimeoutTime"
#define CFG_KEY_SPLIT_PROGRAMS          "SplitPrograms"
#define CFG_KEY_STOP_BITS               "StopBits"
#define CFG_KEY_USE_AS_EXTENSION        "UseAsExtension"
#define CFG_KEY_WAIT_CTS                "WaitForCTS"
#define CFG_KEY_WAIT_XON                "WaitForXON"
#define CFG_KEY_XOFF                    "Xoff"
#define CFG_KEY_XON                     "Xon"
#define CFG_KEY_XON_AT_START            "SendXONAtStart"


SerialPortSettings::SerialPortSettings() :
	BaudRate(QSerialPort::Baud9600),
	DataBits(QSerialPort::Data8),
	Parity(QSerialPort::NoParity),
	StopBits(QSerialPort::TwoStop),
	FlowControl(QSerialPort::HardwareControl),
	configName(""),
#ifdef Q_OS_WIN32
	portName("COM1"),
#else
	portName("/dev/ttyS0"),
#endif
	sendAtEnd(""),
	sendAtBegining(""),
	savePath(""),
	saveExt(".nc"),
	endOfProgChar(""),
	callerProgName("O5555"),
	lineDelay(0.0),
	sendStartDelayReloadValue(0),
	autoCloseTimeout(15),
	sendTimeout(3),
	receiveTimeout(5),
	reconnectTime(60),
	Xon(0x11),
	Xoff(0x13),
	createLogFile(true),
	autoSave(false),
	deleteControlChars(true),
	removeEmptyLines(true),
	removeBefore(false),
	removeSpaceEOB(false),
	renameIfExists(true),
	removeLetters(true),
	guessFileNameByProgName(true),
	appendExt(false),
	useAsExt(false),
	splitPrograms(true),
	fileNameLowerCase(true),
	fileServer(false),
	logData(false),
	waitForCts(false),
	waitForXon(false),
	sendXon(false),
	searchPath1(""),
	searchExt1(".nc"),
	searchPath2(""),
	searchExt2(".nc"),
	searchPath3(""),
	searchExt3(".nc"),
	fileNameExpFs(""),
	fileNameExpAs(""),
	eobChar("CRLF"),
	fileNameExpSaveFile(),
	removeFromRecieved("")
{
}

void SerialPortSettings::load(QSettings* cfg)
{
	if (configName.isEmpty()) {
		qWarning() << "SerialPortSettings.load: configName is empty!";
		return;
	}

	bool ok;
	cfg->beginGroup(configName);
	BaudRate = (QSerialPort::BaudRate) cfg->value(CFG_KEY_BAUD_RATE, BaudRate).toInt();
	DataBits = (QSerialPort::DataBits) cfg->value(CFG_KEY_DATA_BITS, DataBits).toInt();
	Parity = (QSerialPort::Parity) cfg->value(CFG_KEY_PARITY, Parity).toInt();
	StopBits = (QSerialPort::StopBits) cfg->value(CFG_KEY_STOP_BITS, StopBits).toInt();
	FlowControl = (QSerialPort::FlowControl) cfg->value(CFG_KEY_FLOW_CONTROL, FlowControl).toInt();
	portName = cfg->value(CFG_KEY_PORT_NAME, portName).toString();
	sendAtEnd = cfg->value(CFG_KEY_SEND_AT_END, sendAtEnd).toString();
	sendAtBegining = cfg->value(CFG_KEY_SEND_AT_BEGINING, sendAtBegining).toString();
	savePath = cfg->value(CFG_KEY_SAVE_PATH, savePath).toString();
	saveExt = cfg->value(CFG_KEY_SAVE_EXT, saveExt).toString();
	endOfProgChar = cfg->value(CFG_KEY_END_OF_PROG_CHAR, "").toString();
	callerProgName = cfg->value(CFG_KEY_CALLER_PROG, callerProgName).toString();
	lineDelay = cfg->value(CFG_KEY_LINE_DELAY, lineDelay).toDouble();
	sendStartDelayReloadValue = cfg->value(CFG_KEY_SENDING_START_DELAY, 0).toInt();
	autoCloseTimeout = cfg->value(CFG_KEY_AUTO_CLOSE_TIME, autoCloseTimeout).toInt();
	sendTimeout = cfg->value(CFG_KEY_SEND_TIMEOUT, sendTimeout).toInt();
	receiveTimeout = cfg->value(CFG_KEY_RECEIVE_TIMEOUT, receiveTimeout).toInt();
	reconnectTime = cfg->value(CFG_KEY_RECONNECT_TIMEOUT, reconnectTime).toInt();
	// TODO: replace toString().toInt(&ok, 0) with toInt() ?
	Xon = cfg->value(CFG_KEY_XON, Xon).toString().toInt(&ok, 0);
	Xoff = cfg->value(CFG_KEY_XOFF, Xoff).toString().toInt(&ok, 0);
	createLogFile = cfg->value(CFG_KEY_CREATE_LOG_FILE, createLogFile).toBool();
	autoSave = cfg->value(CFG_KEY_AUTO_SAVE, autoSave).toBool();
	deleteControlChars = cfg->value(CFG_KEY_REMOVE_CONTROL_CHARS, deleteControlChars).toBool();
	removeEmptyLines = cfg->value(CFG_KEY_REMOVE_EMPTY_LINES, removeEmptyLines).toBool();
	removeBefore = cfg->value(CFG_KEY_REMOVE_BEFORE, removeBefore).toBool();
	removeSpaceEOB = cfg->value(CFG_KEY_REMOVE_SPACE_EOB, removeSpaceEOB).toBool();
	renameIfExists = cfg->value(CFG_KEY_CREATE_BACKUP, renameIfExists).toBool();
	removeLetters = cfg->value(CFG_KEY_REMOVE_LETTERS, removeLetters).toBool();
	guessFileNameByProgName = cfg->value(CFG_KEY_DETECT_FILE_NAME, guessFileNameByProgName).toBool();
	appendExt = cfg->value(CFG_KEY_APPEND_EXTENSION, appendExt).toBool();
	useAsExt = cfg->value(CFG_KEY_USE_AS_EXTENSION, useAsExt).toBool();
	splitPrograms = cfg->value(CFG_KEY_SPLIT_PROGRAMS, splitPrograms).toBool();
	fileNameLowerCase = cfg->value(CFG_KEY_FILE_NAME_LOWER_CASE, fileNameLowerCase).toBool();
	fileServer = cfg->value(CFG_KEY_FILE_SERVER, fileServer).toBool();
	logData = cfg->value(CFG_KEY_LOG_TO_FILE, logData).toBool();
	waitForCts = cfg->value(CFG_KEY_WAIT_CTS, waitForCts).toBool();
	waitForXon = cfg->value(CFG_KEY_WAIT_XON, waitForXon).toBool();
	sendXon = cfg->value(CFG_KEY_XON_AT_START, sendXon).toBool();
	searchPath1 = cfg->value(CFG_KEY_SEARCH_PATH_1, searchPath1).toString();
	searchExt1 = cfg->value(CFG_KEY_SEARCH_EXT_1, searchExt1).toString();
	searchPath2 = cfg->value(CFG_KEY_SEARCH_PATH_2, searchPath2).toString();
	searchExt2 = cfg->value(CFG_KEY_SEARCH_EXT_2, searchExt2).toString();
	searchPath3 = cfg->value(CFG_KEY_SEARCH_PATH_3, searchPath3).toString();
	searchExt3 = cfg->value(CFG_KEY_SEARCH_EXT_3, searchExt3).toString();
	fileNameExpFs = cfg->value(CFG_KEY_FILE_NAME_EXP_FS, fileNameExpFs).toString();
	fileNameExpAs = cfg->value(CFG_KEY_FILE_NAME_EXP_AS, fileNameExpAs).toString();
	eobChar = cfg->value(CFG_KEY_END_OF_BLOCK_CHAR, eobChar).toString();
	removeFromRecieved = cfg->value(CFG_KEY_REMOVE_FROM_RECEIVED, removeFromRecieved).toString();
	cfg->endGroup();
}

void SerialPortSettings::save(QSettings* cfg)
{
	if (configName.isEmpty()) {
		qWarning() << "SerialPortSettings.save: configName is empty!";
		return;
	}

	cfg->beginGroup(configName);
	cfg->setValue(CFG_KEY_BAUD_RATE, BaudRate);
	cfg->setValue(CFG_KEY_DATA_BITS, DataBits);
	cfg->setValue(CFG_KEY_PARITY, Parity);
	cfg->setValue(CFG_KEY_STOP_BITS, StopBits);
	cfg->setValue(CFG_KEY_FLOW_CONTROL, FlowControl);
	cfg->setValue(CFG_KEY_PORT_NAME, portName);
	cfg->setValue(CFG_KEY_SEND_AT_END, sendAtEnd);
	cfg->setValue(CFG_KEY_SEND_AT_BEGINING, sendAtBegining);
	cfg->setValue(CFG_KEY_SAVE_PATH, savePath);
	cfg->setValue(CFG_KEY_SAVE_EXT, saveExt);
	cfg->setValue(CFG_KEY_END_OF_PROG_CHAR, endOfProgChar);
	cfg->setValue(CFG_KEY_CALLER_PROG, callerProgName);
	cfg->setValue(CFG_KEY_LINE_DELAY, lineDelay);
	cfg->setValue(CFG_KEY_SENDING_START_DELAY, sendStartDelayReloadValue);
	cfg->setValue(CFG_KEY_AUTO_CLOSE_TIME, autoCloseTimeout);
	cfg->setValue(CFG_KEY_SEND_TIMEOUT, sendTimeout);
	cfg->setValue(CFG_KEY_RECEIVE_TIMEOUT, receiveTimeout);
	cfg->setValue(CFG_KEY_RECONNECT_TIMEOUT, reconnectTime);
	cfg->setValue(CFG_KEY_XON, Xon);
	cfg->setValue(CFG_KEY_XOFF, Xoff);
	cfg->setValue(CFG_KEY_CREATE_LOG_FILE, createLogFile);
	cfg->setValue(CFG_KEY_AUTO_SAVE, autoSave);
	cfg->setValue(CFG_KEY_REMOVE_CONTROL_CHARS, deleteControlChars);
	cfg->setValue(CFG_KEY_REMOVE_EMPTY_LINES, removeEmptyLines);
	cfg->setValue(CFG_KEY_REMOVE_BEFORE, removeBefore);
	cfg->setValue(CFG_KEY_REMOVE_SPACE_EOB, removeSpaceEOB);
	cfg->setValue(CFG_KEY_CREATE_BACKUP, renameIfExists);
	cfg->setValue(CFG_KEY_REMOVE_LETTERS, removeLetters);
	cfg->setValue(CFG_KEY_DETECT_FILE_NAME, guessFileNameByProgName);
	cfg->setValue(CFG_KEY_APPEND_EXTENSION, appendExt);
	cfg->setValue(CFG_KEY_USE_AS_EXTENSION, useAsExt);
	cfg->setValue(CFG_KEY_SPLIT_PROGRAMS, splitPrograms);
	cfg->setValue(CFG_KEY_FILE_NAME_LOWER_CASE, fileNameLowerCase);
	cfg->setValue(CFG_KEY_FILE_SERVER, fileServer);
	cfg->setValue(CFG_KEY_LOG_TO_FILE, logData);
	cfg->setValue(CFG_KEY_WAIT_CTS, waitForCts);
	cfg->setValue(CFG_KEY_WAIT_XON, waitForXon);
	cfg->setValue(CFG_KEY_XON_AT_START, sendXon);
	cfg->setValue(CFG_KEY_SEARCH_PATH_1, searchPath1);
	cfg->setValue(CFG_KEY_SEARCH_EXT_1, searchExt1);
	cfg->setValue(CFG_KEY_SEARCH_PATH_2, searchPath2);
	cfg->setValue(CFG_KEY_SEARCH_EXT_2, searchExt2);
	cfg->setValue(CFG_KEY_SEARCH_PATH_3, searchPath3);
	cfg->setValue(CFG_KEY_SEARCH_EXT_3, searchExt3);
	cfg->setValue(CFG_KEY_FILE_NAME_EXP_FS, fileNameExpFs);
	cfg->setValue(CFG_KEY_FILE_NAME_EXP_AS, fileNameExpAs);
	cfg->setValue(CFG_KEY_END_OF_BLOCK_CHAR, eobChar);
	cfg->setValue(CFG_KEY_REMOVE_FROM_RECEIVED, removeFromRecieved);
	cfg->endGroup();
}
