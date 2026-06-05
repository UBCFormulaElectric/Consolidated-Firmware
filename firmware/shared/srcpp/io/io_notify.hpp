#pragma once
#ifdef TARGET_EMBEDDED
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

namespace io::notify
{

// Multi-waiter notification: any number of tasks may call wait(); a single
// notify() unblocks all of them. The latch is auto-cleared once every
// pending waiter has unblocked, so the next wait() blocks until the next
// notify().
class Notifier
{
  public:
    Notifier();

    void wait() const;

    void notify() const;
    void notifyFromISR() const;

#ifdef TARGET_EMBEDDED
  private:
    StaticEventGroup_t storage_{};
    EventGroupHandle_t handle_;
#endif
};

} // namespace io::notify
