#pragma once
#include "FreeRTOS.h"
#include "event_groups.h"
#include "util_errorCodes.hpp"

namespace hw::notify
{

// Multi-waiter notification: any number of tasks may call wait(); a single
// notify() unblocks all of them. The latch is auto-cleared once every
// pending waiter has unblocked, so the next wait() blocks until the next
// notify().
class Notifier
{
  public:
    Notifier();

    void         wait();
    result<void> waitFor(uint32_t timeout_ms);

    void notify();
    void notifyFromISR();

  private:
    StaticEventGroup_t storage_{};
    EventGroupHandle_t handle_;
};

} // namespace hw::notify
