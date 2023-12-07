#include "MainWindow.h"
// libraries
#include <QThread>
#include <chrono>
// can
#include "can.h"

extern "C"
{
#include "Io_CanTx.h"
#include "Io_CanRx.h"
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new ui::MainWindow),
    CanRxTaskThread(QThread::create(&MainWindow::CanRXTask)),
    CanTxPeriodicTaskThread(QThread::create(&MainWindow::CanPeriodicTXTask))
{
    //	setupCanBroadcasting();
    ui->setupUi(this);
}

// Testing purposes (GPIO dummy)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Space:
            ui->toggleSwitcher();
            break;
        case Qt::Key_Return:
            if (ui->getSwitcherOpen())
            {
                ui->toggleSwitcher();
                ui->setSwitcherSelectionToFrame();
            }
            else
            {
            }
            break;
        case Qt::Key_Right:
            ui->switcherRight();
            break;
        case Qt::Key_Left:
            ui->switcherLeft();
            break;
        default:
            qWarning() << QString::fromStdString("UNHANDLED KEY " + event->text().toStdString());
            break;
    }
}

void MainWindow::setupCanBroadcasting()
{
    // CANTX TASK
    tx100Hz.setInterval(10);
    tx100Hz.setSingleShot(false);
    connect(&tx100Hz, &QTimer::timeout, Io_CanTx_Enqueue100HzMsgs);
    tx100Hz.start();

    tx1Hz.setInterval(1000);
    tx1Hz.setSingleShot(false);
    connect(&tx1Hz, &QTimer::timeout, Io_CanTx_Enqueue1HzMsgs);
    tx1Hz.start();

    CanRxTaskThread->start();
    CanTxPeriodicTaskThread->start();
}

[[noreturn]] void MainWindow::CanRXTask()
{
    while (true)
    {
        Result<CanMsg, CanReadError> res = Can_Read();
        if (res.index() == 1)
        {
            switch (get<CanReadError>(res))
            {
                case ReadInterfaceNotCreated:
                    break;
                case SocketReadError:
                case IncompleteCanFrame:
                    continue;
            }
            continue;
        }

        // success
        auto message = get<CanMsg>(res);
        // acquire lock
        Io_CanRx_UpdateRxTableWithMessage(&message);
        // release lock
    }
}

[[noreturn]] void MainWindow::CanPeriodicTXTask()
{
    using namespace std::chrono;
    while (true)
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
    }
}
