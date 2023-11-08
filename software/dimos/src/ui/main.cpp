#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "can.h"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
}

int main(int argc, char *argv[]) {
	//	Io_SharedCan_Init(); // firmware specific i believe

	Io_CanTx_Init(reinterpret_cast<void (*)(const CanMsg *)>(Can_Write));
	//	Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
	//	App_CanTx_Init();
	//	App_CanRx_Init();

	// TODO commit hash
	// App_CanTx_FSM_Hash_Set(GIT_COMMIT_HASH);
	// App_CanTx_FSM_Clean_Set(GIT_COMMIT_CLEAN);

	// ON REPETITION
	// Io_CanTx_EnqueueOtherPeriodicMsgs
	// Io_CanTx_Enqueue100HzMsgs
	// const bool debug_mode_enabled = App_CanRx_Debug_EnableDebugMode_Get();
	// Io_CanTx_Enqueue1HzMsgs();

	// CANRX Task
	// Io_SharedCan_DequeueCanRxMessage(&message);
	// Io_CanRx_UpdateRxTableWithMessage(&message);

	// CANTX TASK
	// Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask();


	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
