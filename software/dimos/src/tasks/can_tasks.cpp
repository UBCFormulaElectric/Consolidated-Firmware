#include <chrono>
#include <QThread>
#include <iostream>
#include <QMutex>

#include "can.h"

extern "C"
{
#include <io_canRx.h>
#include <io_canTx.h>
}

namespace can_handlers
{
QMutex can_table_mutex;

void CanRXTask()
{
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        Result<JsonCanMsg, CanReadError> res = Can_Read();
        if (res.index() == 1)
        {
            switch (get<CanReadError>(res))
            {
                case ReadInterfaceNotCreated:
                    qWarning("[task_CAN] Can interface not created");
                    return;
                case Timeout:
                case SocketReadError:
                case IncompleteCanFrame:
                    break;
            }
            continue;
        }

        // success
        auto message = get<JsonCanMsg>(res);
        // check with
        // io_canRx_filterMessageId
        // if we care about the message
        can_table_mutex.lock();
        io_canRx_updateRxTableWithMessage(&message);
        can_table_mutex.unlock();
    }
    qInfo("[task_CAN] exiting CanRXTask now");
}

void CanPeriodicTXTask()
{
    using namespace std::chrono;
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        can_table_mutex.lock();
        io_canTx_enqueueOtherPeriodicMsgs(ms.count());
        can_table_mutex.unlock();
        QThread::msleep(1); // yield to other threads, make larger if big lag problem
    }
    qInfo("[task_CAN] exiting CanPeriodicTXTask now");
}

void CanTx100Hz()
{
    can_table_mutex.lock();
    io_canTx_enqueue100HzMsgs();
    can_table_mutex.unlock();
}

void CanTx1Hz()
{
    can_table_mutex.lock();
    io_canTx_enqueue1HzMsgs();
    can_table_mutex.unlock();
}
} // namespace can_handlers