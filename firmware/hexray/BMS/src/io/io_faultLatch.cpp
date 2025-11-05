#include "io_faultLatch.hpp"

#include <cassert>

namespace io::faultLatch
{
// BMS_OK is inverted.
const FaultLatch bms_ok_latch{
    &bms_ok_current_status_pin,
    &bms_latch_status_pin,
    true,   // inverted
    false   // read_only
};

const FaultLatch imd_ok_latch{
    &imd_ok_current_status_pin,
    &imd_latch_pin,
    false,  // inverted
    true    // read_only
};

const FaultLatch bspd_ok_latch{
    &bspd_ok_current_status_pin,
    &bspd_ok_latch_status_pin,
    false,  // inverted
    true    // read_only
};

void setCurrentStatus(const FaultLatch* latch, const FaultLatchState status)
{
    assert(latch != nullptr);
    assert(latch->read_only == false);
    hw::gpio::writePin(latch->current_status_gpio, (status == FaultLatchState::OK) ? false : true);
}

FaultLatchState getCurrentStatus(const FaultLatch* latch)
{
    assert(latch != nullptr);
    return (hw::gpio::readPin(latch->current_status_gpio) ^ latch->inverted)
               ? FaultLatchState::OK
               : FaultLatchState::FAULT;
}

FaultLatchState getLatchedStatus(const FaultLatch* latch)
{
    assert(latch != nullptr);
    return hw::gpio::readPin(latch->latch_status_gpio) ? FaultLatchState::OK : FaultLatchState::FAULT;
}
} // namespace io::faultLatch