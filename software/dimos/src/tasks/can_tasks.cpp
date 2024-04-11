#include <chrono>

#include "can.h"
#include "can_tasks.h"

extern "C"
{
#include "io_canRx.h"
#include "io_canTx.h"
#include "app_canRx.h"
}

Result<std::monostate, CAN_setup_errors> CanTask::setup()
{
    Can_Init();
    // tx 100hz
    tx100Hz.setInterval(TASK_INTERVAL_100HZ);
    tx100Hz.setSingleShot(false);
    QObject::connect(&tx100Hz, &QTimer::timeout, &tx100Hzworker, &CanTx100HzTask::run);
    tx100Hz.start();
    // tx 1hz
    tx1Hz.setInterval(TASK_INTERVAL_1HZ);
    tx1Hz.setSingleShot(false);
    QObject::connect(&tx1Hz, &QTimer::timeout, &tx1Hzworker, &CanTx1HzTask::run);
    tx1Hz.start();

    rxworker.start();
    txperiodicworker.start();

    qInfo() << "CAN Threads Successfully Initialized";
    return std::monostate{};
}

void CanTask::CanRxTask::run()
{
    qInfo("Starting CanRXTask thread");
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        Result<JsonCanMsg, CanReadError> res = Can_Read();
        if (res.index() == 1)
        {
            switch (get<CanReadError>(res))
            {
                case CanReadError::ReadInterfaceNotCreated:
                    qWarning("Can interface not created");
                    return;
                case CanReadError::Timeout:
#ifdef USING_TARGET_deploy
                    qWarning("CANRX Timeout");
                    break;
#endif
                case CanReadError::SocketReadError:
                case CanReadError::IncompleteCanFrame:
                    break;
            }
            continue;
        }

        // success
        auto message = get<JsonCanMsg>(res);
        // check with
        // io_canRx_filterMessageId
        // if we care about the message
        can_table_mutex->lock();
        io_canRx_updateRxTableWithMessage(&message);
#ifdef USING_TARGET_dev
        app_canRx_FSM_LeftWheelSpeed_update(10 - app_canRx_FSM_LeftWheelSpeed_get());
        // qInfo("New Dummy Fault Value: %d", app_canRx_VC_Fault_DummyFault_get());
#endif
        can_table_mutex->unlock();
    }
    qInfo("KILL CanRXTask thread");
}

void CanTask::CanTxPeriodicTask::run()
{
    using namespace std::chrono;
    qInfo("Starting CanPeriodicTXTask thread");
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        can_table_mutex->lock();
        io_canTx_enqueueOtherPeriodicMsgs(ms.count());
        can_table_mutex->unlock();
        QThread::msleep(1); // yield to other threads, make larger if big lag problem
    }
    qInfo("KILL CanPeriodicTXTask thread");
}

void CanTask::CanTx100HzTask::run()
{
    can_table_mutex->lock();
    io_canTx_enqueue100HzMsgs();
    can_table_mutex->unlock();
}

void CanTask::CanTx1HzTask::run()
{
    can_table_mutex->lock();
    io_canTx_enqueue1HzMsgs();
    can_table_mutex->unlock();
}

CanTask::~CanTask()
{
    rxworker.requestInterruption();
    txperiodicworker.requestInterruption();
    tx100Hz.stop();
    tx1Hz.stop();
    rxworker.wait();         // TODO figure out what this does
    txperiodicworker.wait(); // TODO figure out what this does
}