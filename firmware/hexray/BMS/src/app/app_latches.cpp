#include "app_latches.hpp"
#include "io_faultLatch.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

void app::latches::broadcast()
{
    using FaultLatchState      = io::FaultLatch::FaultLatchState;
    const bool imd_latched_ok  = imd_ok_latch.getLatchedStatus() == FaultLatchState::OK;
    const bool bspd_latched_ok = bspd_ok_latch.getLatchedStatus() == FaultLatchState::OK;
    const bool bms_latched_ok  = bms_ok_latch.getLatchedStatus() == FaultLatchState::OK;

    app::can_tx::BMS_BmsCurrentlyOk_set(bms_ok_latch.getLatchedStatus() == FaultLatchState::OK);
    app::can_tx::BMS_ImdCurrentlyOk_set(imd_ok_latch.getLatchedStatus() == FaultLatchState::OK);
    app::can_tx::BMS_BspdCurrentlyOk_set(bspd_ok_latch.getLatchedStatus() == FaultLatchState::OK);
    app::can_tx::BMS_BmsLatchOk_set(bms_latched_ok);
    app::can_tx::BMS_ImdLatchOk_set(imd_latched_ok);
    app::can_tx::BMS_BspdLatchOk_set(bspd_latched_ok);
}
