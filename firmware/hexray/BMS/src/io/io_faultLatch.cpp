#include "io_faultLatch.hpp"
#include "hw_gpios.hpp"

#include <cassert>

namespace io::faultLatch
{
// BMS_OK is inverted.
const FaultLatch bms_ok_latch{
    &n_bms_ok_current_status, &n_bms_latch_status,
    true, // current_inverted
    true, // latch_inverted
    false // read_only
};

const FaultLatch imd_ok_latch{
    &imd_ok_current_status, &n_imd_latch_status,
    false, // current_inverted
    true,  // latch_inverted
    true   // read_only
};

const FaultLatch bspd_ok_latch{
    &bspd_ok_current_status, &n_bspd_latch_status,
    true, // current_inverted
    true, // latch_inverted
    true  // read_only
};

void setCurrentStatus(const FaultLatch *latch, const FaultLatchState status)
{
    assert(latch != nullptr);
    assert(latch->read_only == false);
    latch->current_status_gpio->writePin(((status == FaultLatchState::OK) ^ latch->current_inverted) ? true : false);
}

FaultLatchState getCurrentStatus(const FaultLatch *latch)
{
    assert(latch != nullptr);
    return (latch->current_status_gpio->readPin() ^ latch->current_inverted) ? FaultLatchState::OK
                                                                             : FaultLatchState::FAULT;
}

FaultLatchState getLatchedStatus(const FaultLatch *latch)
{
    assert(latch != nullptr);
    return (latch->latch_status_gpio->readPin() ^ latch->latch_inverted) ? FaultLatchState::OK : FaultLatchState::FAULT;
}
} // namespace io::faultLatch
