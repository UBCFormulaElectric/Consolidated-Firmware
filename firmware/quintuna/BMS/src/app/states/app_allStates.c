#include "app_states.h"

#include "app_heartbeatMonitor.h"
#include "app_imd.h"
#include "app_irs.h"
#include "app_powerLimit.h"
#include "app_segments.h"
#include "app_shdnLoop.h"
#include "app_tractiveSystem.h"
#include "io_bspdTest.h"
#include "io_fans.h"
#include "io_faultLatch.h"

#include "app_canRx.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "io_charger.h"

// Time for voltage and cell temperature values to settle
#define CELL_MONITOR_TIME_TO_SETTLE_MS (300U)

static TimerChannel cell_monitor_settle_timer;

void app_allStates_init(void)
{
    app_timer_init(&cell_monitor_settle_timer, CELL_MONITOR_TIME_TO_SETTLE_MS);
    app_timer_restart(&cell_monitor_settle_timer);
}

void app_allStates_runOnTick100Hz(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

    // app_heartbeatMonitor_checkIn(&hb_monitor);
    // app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_shdnLoop_broadcast();
    app_powerLimit_broadcast();

    // TODO: Enable fans for endurance when contactors are closed.
    // const bool hv_up = io_irs_isNegativeClosed() && io_irs_isPositiveClosed();
    // io_fans_tick(hv_up);
    io_fans_tick(false);

    io_bspdTest_enable(app_canRx_Debug_EnableTestCurrent_get());
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(io_bspdTest_isCurrentThresholdExceeded());

    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    app_canTx_BMS_ChargerConnectedType_set(io_charger_getConnectionStatus());

    (void)app_segments_checkWarnings();
    const bool acc_fault = app_segments_checkFaults();
    io_faultLatch_setCurrentStatus(&bms_ok_latch, acc_fault ? FAULT_LATCH_FAULT : FAULT_LATCH_OK);

    // Update CAN signals for BMS latch statuses.
    app_canTx_BMS_BmsCurrentlyOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_ImdCurrentlyOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BspdCurrentlyOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BmsLatchOk_set(io_faultLatch_getLatchedStatus(&bms_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_ImdLatchOk_set(io_faultLatch_getLatchedStatus(&imd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BspdLatchOk_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch) == FAULT_LATCH_OK);

    app_canTx_BMS_BSPDBrakePressureThresholdExceeded_set(io_bspdTest_isBrakePressureThresholdExceeded());
    app_canTx_BMS_BSPDAccelBrakeOk_set(io_bspdTest_isAccelBrakeOk());
    // Wait for cell voltage and temperature measurements to settle. We expect to read back valid values from the
    // monitoring chips within 3 cycles
    // const bool settle_time_expired = app_timer_updateAndGetState(&cell_monitor_settle_timer) == TIMER_STATE_EXPIRED;
    // if (acc_fault && settle_time_expired)
    // {
    //     app_stateMachine_setNextState(app_faultState_get());
    // }
}