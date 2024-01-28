#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "can.h"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_canRx.h"
#include "io_canTx.h"
}

const bool GIT_COMMIT_HASH = true;
const bool GIT_COMMIT_CLEAN = false;

int main(int argc, char *argv[]) {
	// IO init
	io_canTx_init(reinterpret_cast<void (*)(const JsonCanMsg *)>(Can_Write));
	io_canTx_enableMode(CAN_MODE_DEFAULT, true);
	// clear tables
	app_canTx_init();
	app_canRx_init();
	// TODO commit hash
	app_canTx_dimos_Hash_set(GIT_COMMIT_HASH);
	app_canTx_dimos_Clean_set(GIT_COMMIT_CLEAN);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
