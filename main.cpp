#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
	QApplication::setApplicationName("PrintFile");
	QApplication::setOrganizationName("Benjamin Lutz");
	QApplication::setOrganizationDomain("printfile.maxlor.com");
	QApplication::setWindowIcon(QIcon(":/printer.svg"));
	
	QApplication a(argc, argv);
	MainWindow w;
	return a.exec();
}
