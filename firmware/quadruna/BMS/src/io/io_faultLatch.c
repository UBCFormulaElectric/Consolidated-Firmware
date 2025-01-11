#include "io_faultLatch.h"

const FaultLatch bms_ok_latch = {
    .current_status_gpio = &bms_ok_current_status_gpio,
    .latch_status_gpio   = &bms_ok_latch_status_gpio,
    .read_only           = false,
};
const FaultLatch imd_ok_latch = {
    .current_status_gpio = &imd_ok_current_status_gpio,
    .latch_status_gpio   = &imd_ok_latch_status_gpio,
    .read_only           = true,
};
const FaultLatch bspd_ok_latch = {
    .current_status_gpio = &bspd_ok_current_status_gpio,
    .latch_status_gpio   = &bspd_ok_latch_status_gpio,
    .read_only           = true,
};

void io_faultLatch_setCurrentStatus(const FaultLatch *latch, const bool status)
{
    if (latch->read_only == false)
    {
        hw_gpio_writePin(latch->current_status_gpio, status);
    }
}

bool io_faultLatch_getCurrentStatus(const FaultLatch *latch)
{
    return hw_gpio_readPin(latch->current_status_gpio);
}

bool io_faultLatch_getLatchedStatus(const FaultLatch *latch)
{
    return !hw_gpio_readPin(latch->latch_status_gpio);
}
