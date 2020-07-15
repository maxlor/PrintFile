#ifndef PRINTER_H
#define PRINTER_H

#include <QAbstractSocket>
#include <QIODevice>
#include <QObject>
#include <QTcpSocket>

class Printer : public QObject {
	Q_OBJECT
public:
	// Busy means we can't test right now because we're doing something already
	enum class Result { OK, NoResponse, ConnectionRefused, InvalidAddress, InvalidPort,
		                    Unknown, Busy }; 
	
	explicit Printer(QObject *parent = nullptr);
	
	const QString address() const { return _address; }
	int port() const { return _port; }
	
	void test();
	void print(QIODevice *data);
	
signals:
	void printResult(Result result);
	void testResult(Result result);
	
public slots:
	void setAddress(const QString &address);
	void setPort(int port);
	
private slots:
	void socketStateChanged(QAbstractSocket::SocketState state);
	
private:
	Result result(QAbstractSocket::SocketError error);
	void sendData();
	enum class Mode { None, Test, Print };
	
	QString _address;
	int _port = -1;
	Mode _mode = Mode::None;
	QTcpSocket _socket;
	bool _connectionEstablished;
	QIODevice *_data = nullptr;
};

#endif // PRINTER_H
