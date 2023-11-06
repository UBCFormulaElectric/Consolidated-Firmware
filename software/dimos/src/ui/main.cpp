#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "App_CanTx.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
