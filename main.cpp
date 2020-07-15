#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

#define STR(x) _STR(x)
#define _STR(x) #x


int main(int argc, char *argv[]) {
	QApplication::setApplicationName("PrintFile");
    QApplication::setApplicationVersion(STR(APP_VERSION));
	QApplication::setOrganizationName("Benjamin Lutz");
	QApplication::setOrganizationDomain("printfile.maxlor.com");
	QApplication::setWindowIcon(QIcon(":/printer.svg"));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	
	QApplication a(argc, argv);
    MainWindow w;
	return a.exec();
}
