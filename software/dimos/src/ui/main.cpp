#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "can.h"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "Io_CanRx.h"
#include "Io_CanTx.h"
}

const bool GIT_COMMIT_HASH = true;
const bool GIT_COMMIT_CLEAN = false;

int main(int argc, char *argv[]) {
	// IO init
	Io_CanTx_Init(reinterpret_cast<void (*)(const JsonCanMsg *)>(Can_Write));
	Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
	// clear tables
	App_CanTx_Init();
	App_CanRx_Init();
	// TODO commit hash
	App_CanTx_dimos_Hash_Set(GIT_COMMIT_HASH);
	App_CanTx_dimos_Clean_Set(GIT_COMMIT_CLEAN);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
