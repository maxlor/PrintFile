#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QString>
#include <QSystemTrayIcon>
#include <QTimer>
#include "printer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
protected:
	virtual void closeEvent(QCloseEvent *event) override;
	
private slots:
	void savePrinter();
	void chooseWatchFile();
	void watchTimeout();
	void enableWatchChanged(bool enabled);
    void watchPathChanged();
	
	void printResult(Printer::Result result);
	void testResult(Printer::Result result);
	
	void trayIconActivated();
	
	void on_actionAbout_triggered();
	void on_actionPrintFile_triggered();
	void on_actionQuit_triggered();
	void on_actionTest_triggered();
	
private:
    void showLicense();
	QString selectFile(const QString &caption = QString());
	bool minimizeToTray();
	
	Ui::MainWindow *ui;
	
	QString _currentDir;
	Printer *_printer;
	QFile *_file = nullptr;
	QTimer _watchTimer;
	bool _clearFile = false;
	bool _quitOnClose = false;
	QSystemTrayIcon *_trayIcon = nullptr;
};
#endif // MAINWINDOW_H
