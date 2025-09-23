#include "io_faultLatch.h"
#include <assert.h>

// BMS_OK is inverted.
const FaultLatch bms_ok_latch = {
    .current_status_gpio = &bms_ok_current_status_pin,
    .latch_status_gpio   = &bms_latch_status_pin,
    .read_only           = false,
};
const FaultLatch imd_ok_latch = {
    .current_status_gpio = &imd_ok_current_status_pin,
    .latch_status_gpio   = &imd_latch_pin,
    .read_only           = true,
};
const FaultLatch bspd_ok_latch = {
    .current_status_gpio = &bspd_ok_current_status_pin,
    .latch_status_gpio   = &bspd_ok_latch_status_pin,
    .read_only           = true,
};

void io_faultLatch_setCurrentStatus(const FaultLatch *latch, const FaultLatchState status)
{
    assert(latch->read_only == false);
    hw_gpio_writePin(latch->current_status_gpio, status == FAULT_LATCH_OK ? true : false);
}

FaultLatchState io_faultLatch_getCurrentStatus(const FaultLatch *latch)
{
    return hw_gpio_readPin(latch->current_status_gpio) ? FAULT_LATCH_OK : FAULT_LATCH_FAULT;
}

FaultLatchState io_faultLatch_getLatchedStatus(const FaultLatch *latch)
{
    return !hw_gpio_readPin(latch->latch_status_gpio) ? FAULT_LATCH_OK : FAULT_LATCH_FAULT;
}
