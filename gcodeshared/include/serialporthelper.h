#ifndef SERIALPORTHELPER_H
#define SERIALPORTHELPER_H

#include <QObject>

#include <serialportsettings.h>

class SerialPortHelper : public QObject
{
	Q_OBJECT

public:
	explicit SerialPortHelper(const SerialPortSettings& cfg, QObject* parent = nullptr);
	~SerialPortHelper();

	bool connect();

	QSerialPort::SerialPortError error() const;
	QString errorString() const;

	void putChar(const char& ch);
	void setDataTerminalReady(bool ready);
	void setRequestToSend(bool req);

	void sendText(const QString& text);
signals:

protected:
	QSerialPort* comPort;
	SerialPortSettings m_config;
};

#endif // SERIALPORTHELPER_H
