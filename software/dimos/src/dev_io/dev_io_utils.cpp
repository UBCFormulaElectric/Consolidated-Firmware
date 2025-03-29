#include "dev_io_utils.h"

#include <QThread>
#include <thread>

static constexpr int TIMEOUT_TIME_MSEC = 3000;
static constexpr int RESPONSE_TIME     = 30;
static constexpr int CYCLES = TIMEOUT_TIME_MSEC / RESPONSE_TIME; // CYCLES * RESPONSE_TIME = TIMEOUT_TIME_MSEC

WaitDelegateResult wait_delegate_thread()
{
    for (int i = 0; i < CYCLES; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(RESPONSE_TIME));
        if (QThread::currentThread()->isInterruptionRequested())
            return WaitDelegateResult::INTERRUPTED;
    }

    return WaitDelegateResult::TIMEOUT;
}