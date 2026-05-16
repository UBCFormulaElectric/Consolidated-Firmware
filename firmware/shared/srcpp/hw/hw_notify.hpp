#pragma once
#include "FreeRTOS.h"
#include "task.h"

namespace hw::notify
{

// One task calls wait() to block; another calls notifyIfWaiting() to unblock it
// Only notifies if a task has registered interest
class Notifier
{
  public:
    void wait();

    void notifyIfWaiting();

    void notifyIfWaitingFromISR();

  private:
    volatile TaskHandle_t waitingTask_{ nullptr };
};

} // namespace hw::notify
