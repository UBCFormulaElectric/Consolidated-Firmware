#pragma once
#include "FreeRTOS.h"
#include "task.h"
#include "util_errorCodes.hpp"

namespace hw::notify
{

constexpr uint32_t DEFAULT_NOTIFY_TIMEOUT_MS = 1000U;

// One task calls wait() to block; another calls notifyIfWaiting() to unblock it
// Only notifies if a task has registered interest
class Notifier
{
  public:
    void         wait();
    result<void> waitFor(uint32_t timeout_ms = DEFAULT_NOTIFY_TIMEOUT_MS);

    void notifyIfWaiting();

    void notifyIfWaitingFromISR();

  private:
    volatile TaskHandle_t waitingTask_{ nullptr };
};

} // namespace hw::notify
