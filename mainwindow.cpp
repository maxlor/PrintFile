#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QtDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	QSettings settings;
	_currentDir = settings.value("currentdir").toString();
	QRect geometry = settings.value("geometry").toRect();
	if (geometry.isValid()) { setGeometry(geometry); }
	ui->editAddress->setText(settings.value("address").toString());
	ui->editPort->setValue(settings.value("port", 9100).toInt());
	ui->labelStatus->setText("");
	ui->buttonTestPrinter->setDefaultAction(ui->actionTest);
	ui->groupFile->setChecked(settings.value("watchenabled").toBool());
	ui->editWatchPath->setText(settings.value("watchfile").toString());
	
	_printer = new Printer(this);
	_printer->setAddress(ui->editAddress->text());
	_printer->setPort(ui->editPort->value());
	_watchTimer.setInterval(2000);
	_watchTimer.setSingleShot(false);
	_trayIcon = new QSystemTrayIcon(QIcon(":/printer.png"), this);
	
	connect(ui->editAddress, &QLineEdit::textChanged, this, &MainWindow::savePrinter);
	connect(ui->editPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        this, &MainWindow::savePrinter);
	connect(ui->groupFile, &QGroupBox::clicked, this, &MainWindow::enableWatchChanged);
	connect(ui->buttonChooseFile, &QAbstractButton::clicked, this, &MainWindow::chooseWatchFile);
	connect(_printer, &Printer::printResult, this, &MainWindow::printResult);
	connect(_printer, &Printer::testResult, this, &MainWindow::testResult);
	connect(&_watchTimer, &QTimer::timeout, this, &MainWindow::watchTimeout);
	connect(_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
	
	if (minimizeToTray()) {
		_trayIcon->setVisible(true);
	} else {
		show();
	}
}


MainWindow::~MainWindow() {
	delete ui;
	QSettings settings;
	settings.setValue("geometry", geometry());
}


void MainWindow::closeEvent(QCloseEvent *event) {
	if (minimizeToTray() and not _quitOnClose) {
		_trayIcon->setVisible(true);
		hide();
		event->ignore();
	} else {
		QMainWindow::closeEvent(event);
	}
}


void MainWindow::savePrinter() {
	const QString address = ui->editAddress->text();
	const int port = ui->editPort->value();
	        
	_printer->setAddress(address);
	_printer->setPort(port);
	
	QSettings settings;
	settings.setValue("address", address);
	settings.setValue("port", port);
}


void MainWindow::chooseWatchFile() {
	const QString path = selectFile(tr("Select Watch File"));
	if (path.isEmpty()) { return; }
	
	ui->editWatchPath->setText(path);
	QSettings().setValue("watchfile", path);
}


void MainWindow::watchTimeout() {
	if (_file) { return; }
	
	const QString &path = ui->editWatchPath->text();
	if (path.isEmpty()) { return; }
	
	_file = new QFile(path, this);
	
	QFileInfo fi(*_file);
	int age = fi.fileTime(QFile::FileModificationTime).msecsTo(QDateTime::currentDateTime());
	if (fi.size() == 0 or age < 3000) {
		delete _file;
		_file = nullptr;
		return;
	}
	
	if (not _file->open(QIODevice::ReadWrite)) {
		delete _file;
		_file = nullptr;
		return;
	}
	
	_clearFile = true;
	_printer->print(_file);
}


void MainWindow::enableWatchChanged(bool enabled) {
	if (enabled) {
		_watchTimer.start();
	} else {
		_watchTimer.stop();
	}
	QSettings().setValue("watchenabled", enabled);
}


void MainWindow::printResult(Printer::Result result) {
	if (_clearFile) {
		_file->seek(0);
		_file->resize(0);
		_clearFile = false;
	}
	_file->close();
	delete _file;
	_file = nullptr;
	
	auto showError = [&](const QString &reason) {
		QMessageBox::critical(this, tr("Cannot Print"), tr("Cannot print: %1").arg(reason));
	};
	
	switch (result) {
	case Printer::Result::OK:
		ui->statusbar->showMessage(tr("printing successful"), 5000);
		break;
	case Printer::Result::NoResponse:
		showError(tr("no response from the printer."));
		break;
	case Printer::Result::ConnectionRefused:
		showError(tr("the printer refused the connection."));
		break;
	case Printer::Result::InvalidAddress:
		showError(tr("the address is invalid"));
		break;
	case Printer::Result::InvalidPort:
		showError(tr("the port is invalid"));
		break;
	case Printer::Result::Unknown:
		ui->labelStatus->setText(tr("network error"));
		break;
	case Printer::Result::Busy:
		ui->labelStatus->setText(tr("busy"));
		break;
	}
}


void MainWindow::testResult(Printer::Result result) {
	ui->actionTest->setEnabled(true);
	
	qDebug() << Q_FUNC_INFO << (int)result;
	
	switch (result) {
	case Printer::Result::OK:
		ui->labelStatus->setText(tr("OK"));
		break;
	case Printer::Result::NoResponse:
		ui->labelStatus->setText(tr("no response"));
		break;
	case Printer::Result::ConnectionRefused:
		ui->labelStatus->setText(tr("connection refused"));
		break;
	case Printer::Result::InvalidAddress:
		ui->labelStatus->setText(tr("invalid address"));
		break;
	case Printer::Result::InvalidPort:
		ui->labelStatus->setText(tr("invalid port"));
		break;
	case Printer::Result::Unknown:
		ui->labelStatus->setText(tr("network error"));
		break;
	case Printer::Result::Busy:
		ui->labelStatus->setText(tr("busy"));
		break;
	}
}


void MainWindow::trayIconActivated() {
	show();
	_trayIcon->setVisible(false);
}


void MainWindow::on_actionAbout_triggered() {
	QMessageBox::about(this, QString(),
	                   tr("<html><p>&copy; 2020 Benjamin Lutz</p>"
	                      "<p>This program sends files to a printer's AppSocket port. "
	                      "It's intended to be used in conjunction with VMware Player's "
	                      "capability to redirect a parallel port to a file.</p>"
	                      "<p>Icon made by Freepik from www.flaticon.com.</p>"));
}


void MainWindow::on_actionPrintFile_triggered() {
	QString path = selectFile(tr("Print File"));
	if (path.isEmpty()) { return; }
	
	_file = new QFile(path, this);
	if (not _file->open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, tr("Cannot Open File"),
		                      tr("Cannot open file: %1").arg(_file->errorString()));
		return;
	}
	
	_printer->print(_file);
}


void MainWindow::on_actionQuit_triggered() {
	_quitOnClose = true;
	close();
}


void MainWindow::on_actionTest_triggered() {
	ui->actionTest->setEnabled(false);
	ui->labelStatus->setText("...");
	_printer->test();
}


QString MainWindow::selectFile(const QString &caption) {
	const QString path = QFileDialog::getOpenFileName(this, caption, _currentDir);
	if (not path.isEmpty()) {
		QFileInfo fi(path);
		_currentDir = fi.absolutePath();
		QSettings().setValue("currentdir", _currentDir);
	}
	return path;
}


bool MainWindow::minimizeToTray() {
	return QSystemTrayIcon::isSystemTrayAvailable() and ui->groupFile->isChecked();
}

