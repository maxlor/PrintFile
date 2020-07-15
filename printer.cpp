#include "printer.h"


Printer::Printer(QObject *parent) : QObject(parent) {
	connect(&_socket, &QTcpSocket::stateChanged, this, &Printer::socketStateChanged);
	connect(&_socket, &QTcpSocket::bytesWritten, this, &Printer::sendData);
}


void Printer::setAddress(const QString &address) {
	_address = address;
}


void Printer::setPort(int port) {
	_port = port;
}


void Printer::test() {
	if (_mode != Mode::None) {
		emit testResult(Result::Busy);
		return;
	}
	
	_mode = Mode::Test;
	_connectionEstablished = false;
	
	if (_port < 0 or _port > 65535) {
		emit testResult(Result::InvalidPort);
		return;
	}
	
	_socket.connectToHost(_address, _port);
}


void Printer::print(QIODevice *data) {
	if (_mode != Mode::None) {
		emit printResult(Result::Busy);
		return;
	}
	
	_mode = Mode::Print;
	_data = data;
	_socket.connectToHost(_address, _port);
}


void Printer::socketStateChanged(QAbstractSocket::SocketState state) {
	switch (state) {
	case QAbstractSocket::ConnectedState:
		_connectionEstablished = true;
		if (_mode == Mode::Print) {
			sendData();
		} else if (_mode == Mode::Test) {
			_socket.close();
		}
		break;
	case QAbstractSocket::UnconnectedState:
		if (_mode == Mode::Print) {
			if (_connectionEstablished) {
				emit printResult(Result::OK);
			} else {
				emit printResult(result(_socket.error()));
			}
		} else if (_mode == Mode::Test) {
			if (_connectionEstablished) {
				emit testResult(Result::OK);
			} else {
				emit testResult(result(_socket.error()));
			}
		}
		_data = nullptr;
		_mode = Mode::None;
		break;
	case QAbstractSocket::HostLookupState:
	case QAbstractSocket::ConnectingState:
	case QAbstractSocket::ClosingState:
		break;
	case QAbstractSocket::BoundState:
	case QAbstractSocket::ListeningState:
		Q_UNREACHABLE();
		break;
	}
}


Printer::Result Printer::result(QAbstractSocket::SocketError error) {
	switch (error) {
	case QAbstractSocket::ConnectionRefusedError: return Result::ConnectionRefused;
	case QAbstractSocket::HostNotFoundError: return Result::InvalidAddress;
	case QAbstractSocket::SocketTimeoutError: return Result::NoResponse;
	case QAbstractSocket::NetworkError: return Result::NoResponse;
	default: return Result::Unknown;
	}
}


void Printer::sendData() {
	if (_data) {
		if (_data->atEnd()) {
			_data = nullptr;
			_socket.close();
			return;
		}
		
		_socket.write(_data->read(10000));
	} else {
		Q_UNREACHABLE();
	}
}
