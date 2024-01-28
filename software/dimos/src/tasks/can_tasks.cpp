#include <chrono>
#include <QThread>
#include <iostream>
#include <QMutex>

#include "can.h"

extern "C"
{
#include <Io_CanRx.h>
#include <Io_CanTx.h>
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
                    qWarning("Can interface not created");
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
        // Io_CanRx_FilterMessageId
        // if we care about the message
        can_table_mutex.lock();
        Io_CanRx_UpdateRxTableWithMessage(&message);
        can_table_mutex.unlock();
    }
    std::cout << "exiting CanRXTask now" << std::endl;
}

void CanPeriodicTXTask()
{
    using namespace std::chrono;
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        can_table_mutex.lock();
        Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
        can_table_mutex.unlock();
        QThread::msleep(1); // yield to other threads, make larger if big lag problem
    }
    std::cout << "exiting CanPeriodicTXTask now" << std::endl;
}

void CanTx100Hz()
{
    can_table_mutex.lock();
    Io_CanTx_Enqueue100HzMsgs();
    can_table_mutex.unlock();
}

void CanTx1Hz()
{
    can_table_mutex.lock();
    Io_CanTx_Enqueue1HzMsgs();
    can_table_mutex.unlock();
}
} // namespace can_handlers