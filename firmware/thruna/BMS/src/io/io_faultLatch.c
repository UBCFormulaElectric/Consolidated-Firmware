#include "io_faultLatch.h"

void io_faultLatch_setCurrentStatus(const FaultLatch * latch, bool status)
{
    if (!latch->read_only)
    {
        hw_gpio_writePin(&latch->current_status_gpio, status);
    }
}

bool io_faultLatch_getCurrentStatus(const FaultLatch * latch)
{
    return hw_gpio_readPin(&latch->current_status_gpio);
}

bool io_faultLatch_getLatchedStatus(const FaultLatch * latch)
{
    return hw_gpio_readPin(&latch->latch_status_gpio);
}
