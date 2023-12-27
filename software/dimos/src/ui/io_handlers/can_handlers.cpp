#include <chrono>
#include <QThread>
#include <iostream>

#include "can.h"

extern "C"
{
#include <Io_CanRx.h>
}

namespace can_handlers
{
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
        // acquire lock
        Io_CanRx_UpdateRxTableWithMessage(&message);
        // release lock
    }
    std::cout << "exiting CanRXTask now" << std::endl;
}

void CanPeriodicTXTask()
{
    using namespace std::chrono;
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
    }
    std::cout << "exiting CanPeriodicTXTask now" << std::endl;
}
} // namespace can_handlers