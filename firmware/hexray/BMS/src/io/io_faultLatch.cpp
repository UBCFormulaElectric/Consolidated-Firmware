#include "io_faultLatch.hpp"
#include "hw_gpios.hpp"

#include <cassert>

namespace io
{
void FaultLatch::setCurrentStatus(const FaultLatchState status) const
{
    assert(latch->read_only == false);
    current_status_gpio.writePin((status == FaultLatchState::OK) ^ current_inverted);
}

FaultLatch::FaultLatchState FaultLatch::getCurrentStatus() const
{
    return current_status_gpio.readPin() ^ current_inverted ? FaultLatchState::OK : FaultLatchState::FAULT;
}

FaultLatch::FaultLatchState FaultLatch::getLatchedStatus() const
{
    return latch_status_gpio.readPin() ^ latch_inverted ? FaultLatchState::OK : FaultLatchState::FAULT;
}
} // namespace io

// BMS_OK is inverted.
const io::FaultLatch bms_ok_latch{
    n_bms_ok_current_status, n_bms_latch_status,
    true, // current_inverted
    true, // latch_inverted
    false // read_only
};

const io::FaultLatch imd_ok_latch{
    imd_ok_current_status, n_imd_latch_status,
    false, // current_inverted
    true,  // latch_inverted
    true   // read_only
};

const io::FaultLatch bspd_ok_latch{
    bspd_ok_current_status, n_bspd_latch_status,
    true, // current_inverted
    true, // latch_inverted
    true  // read_only
};
