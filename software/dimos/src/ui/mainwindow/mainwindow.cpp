    #include <QThread>
#include <chrono>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "can.h"

extern "C" {
#include "Io_CanTx.h"
#include "Io_CanRx.h"
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new ui::MainWindow) {
	setupCan();
	ui->setupUi(this);
	l = std::make_unique<LandingPage>(this);
}

void MainWindow::setupCan() {
	// CANTX TASK
	tx100Hz.setInterval(10);
	tx100Hz.setSingleShot(false);
	QTimer::connect(&tx100Hz, &QTimer::timeout, Io_CanTx_Enqueue100HzMsgs);
	tx100Hz.start();

	tx1Hz.setInterval(1000);
	tx1Hz.setSingleShot(false);
	QTimer::connect(&tx1Hz, &QTimer::timeout, Io_CanTx_Enqueue1HzMsgs);
	tx1Hz.start();

	CanRxTaskThread = QThread::create(&MainWindow::CanRXTask);
	CanTxPeriodicTaskThread = QThread::create(&MainWindow::CanPeriodicTXTask);
	CanRxTaskThread->start();
	CanTxPeriodicTaskThread->start();
}

[[noreturn]] void MainWindow::CanRXTask() {
	while (true) {
		Result<CanMsg, CanReadError> res = Can_Read();
		if (res.index() == 1) {
			switch (get<CanReadError>(res)) {
				case ReadInterfaceNotCreated:
					break;
				case SocketReadError:
				case IncompleteCanFrame:
					continue;
			}
			continue;
		}

		// success
		CanMsg message = get<CanMsg>(res);
		// acquire lock
		Io_CanRx_UpdateRxTableWithMessage(&message);
		// release lock
	}
}

[[noreturn]] void MainWindow::CanPeriodicTXTask() {
	using namespace std::chrono;
	while (true) {
		auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
	}
}

MainWindow::~MainWindow() = default;