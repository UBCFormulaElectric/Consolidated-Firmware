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

QMutex rx_mutex, tx_mutex;

void CanRXTask()
{
    while(!QThread::currentThread()->isInterruptionRequested())
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
        //check with
        // Io_CanRx_FilterMessageId
        // if we care about the message
        rx_mutex.lock();
        Io_CanRx_UpdateRxTableWithMessage(&message);
        rx_mutex.unlock();
    }
    std::cout << "exiting CanRXTask now" << std::endl;
}

void CanPeriodicTXTask()
{
    using namespace std::chrono;
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        tx_mutex.lock();
        Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
        tx_mutex.unlock();
    }
    std::cout << "exiting CanPeriodicTXTask now" << std::endl;
}


void CanTx100Hz() {
    tx_mutex.lock();
    Io_CanTx_Enqueue100HzMsgs();
    tx_mutex.unlock();
}

void CanTx1Hz() {
    tx_mutex.lock();
    Io_CanTx_Enqueue1HzMsgs();
    tx_mutex.unlock();
}
} // namespace can_handlers