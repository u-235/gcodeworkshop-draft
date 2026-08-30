#include "../include/serialporthelper.h"

SerialPortHelper::SerialPortHelper(const SerialPortSettings& cfg, QObject* parent)
	: QObject{parent}
{}

SerialPortHelper::~SerialPortHelper()
{
	if (comPort != nullptr) {
		comPort->close();
		delete (comPort);
	}
}

bool SerialPortHelper::connect()
{
	if (comPort != nullptr) {
		comPort->reset();
		comPort->close();
		comPort->deleteLater();
	}

	comPort = new QSerialPort(m_config.portName, this);
	comPort->clearError();
	comPort->setBaudRate(m_config.BaudRate);
	comPort->setDataBits(m_config.DataBits);
	comPort->setParity(m_config.Parity);
	comPort->setStopBits(m_config.StopBits);
	comPort->setFlowControl(m_config.FlowControl);

	if (!comPort->open(QIODevice::ReadWrite)) {
		comPort->deleteLater();
		comPort = nullptr;
		return false;
	}

	comPort->clear(QSerialPort::AllDirections);
	return true;
}

QSerialPort::SerialPortError SerialPortHelper::error() const
{
	if (comPort != nullptr) {
		return comPort->error();
	} else {
		return  QSerialPort::NoError;
	}
}

QString SerialPortHelper::errorString() const
{
	if (comPort != nullptr) {
		return comPort->errorString();
	} else {
		return QString();
	}
}

void SerialPortHelper::putChar(const char& ch)
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->putChar(ch);
	}
}

void SerialPortHelper::setDataTerminalReady(bool ready)
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->setDataTerminalReady(ready);
	}
}

void SerialPortHelper::setRequestToSend(bool req)
{
	if (comPort != nullptr && comPort->isOpen()) {
		comPort->setRequestToSend(req);
	}
}

void SerialPortHelper::sendText(const QString& text)
{
	QString tx = text;
	int i;
	bool xoffReceived;
	char controlChar;
	int bytesToWrite;

	if (comPort == nullptr) {
		return;
	}

	if (comPort->isOpen()) {
		tx.prepend(m_config.sendAtBegining);
		tx.append(m_config.sendAtEnd);

		if (!tx.contains("\r\n")) {
			tx.replace("\n", "\r\n");
		}

		errorLabel->setText(tr("Waiting..."));
		qApp->processEvents();

		i = 0;
		xoffReceived = true;

		while (i < tx.size()) {
			if (xoffReceived) {
				errorLabel->setText(tr("Waiting for a signal readiness..."));
			}

			qApp->processEvents();

			if (stop) {
				break;
			}

			if (m_config.FlowControl == QSerialPort::SoftwareControl) {
				controlChar = 0;

				if (comPort->bytesAvailable() > 0) {
					comPort->getChar(&controlChar);
				}

				if (controlChar == m_config.Xoff) {
					xoffReceived = true;
				}

				if (controlChar == m_config.Xon) {
					xoffReceived = false;
				}

				//setXoffButton->setChecked(xoffReceived);
				//setXonButton->setChecked(!xoffReceived);
			} else {
				xoffReceived = false;
			}

			bytesToWrite = comPort->bytesToWrite();

			if ((bytesToWrite == 0) && (!xoffReceived)) {
				if (!comPort->putChar(tx[i].toLatin1())) {
					showError(comPort->error());
				}

				errorLabel->setText(tr("Sending byte %1 of %2").arg(i + 1).arg(tx.size()));
				qApp->processEvents();

				if (m_config.lineDelay > 0) {
					if (tx[i].toLatin1() == '\n') {
						readyCont = false;
						QTimer::singleShot(int(m_config.lineDelay * 1000), this, SLOT(lineDelaySlot()));

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
