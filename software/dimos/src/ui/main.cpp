#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "can.h"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
