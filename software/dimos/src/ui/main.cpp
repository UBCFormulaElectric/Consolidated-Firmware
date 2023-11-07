#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "App_CanTx.h"
#include "App_CanRx.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	App_CanTx_Init();
	return QApplication::exec();
}
