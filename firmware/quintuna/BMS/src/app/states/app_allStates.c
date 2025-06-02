#include "app_allStates.h"
#include "app_accumulator.h"
#include "app_heartbeatMonitor.h"
#include "app_heartbeatMonitors.h"
#include "app_imd.h"
#include "app_irs.h"
#include "app_shdnLoop.h"
#include "app_thermistors.h"
#include "app_tractiveSystem.h"
#include "io_bspdTest.h"
#include "io_faultLatch.h"
#include "states/app_faultState.h"
#include <app_canRx.h>
#include <app_canTx.h>

// Time for voltage and cell temperature values to settle
#define CELL_MONITOR_TIME_TO_SETTLE_MS (300U)

static TimerChannel cell_monitor_settle_timer;

void app_allStates_init(void)
{
    app_timer_init(&cell_monitor_settle_timer, CELL_MONITOR_TIME_TO_SETTLE_MS);
    app_timer_restart(&cell_monitor_settle_timer);
}

void app_allStates_runOnTick1Hz(void) {}

void app_allStates_runOnTick100Hz(void)
{
    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    app_accumulator_broadcast();
    app_thermistors_updateAuxThermistorTemps();
    app_thermistors_broadcast();
    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_irs_broadcast();
    app_shdnLoop_broadcast();

    const bool bspd_test_current_enable = app_canRx_Debug_EnableTestCurrent_get();
    io_bspdTest_enable(bspd_test_current_enable);
    const bool bspd_current_threshold_exceeded = io_bspdTest_isCurrentThresholdExceeded();
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(bspd_current_threshold_exceeded);

    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    bool charger_is_connected = false; // TODO: Charger app_canRx_BRUSA_IsConnected_get();
    app_canTx_BMS_ChargerConnected_set(charger_is_connected);

    // Update CAN signals for BMS latch statuses.
    app_canTx_BMS_BmsOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch));
    app_canTx_BMS_ImdOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch));
    app_canTx_BMS_BspdOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch));
    app_canTx_BMS_BmsLatchedFault_set(io_faultLatch_getLatchedStatus(&bms_ok_latch));
    app_canTx_BMS_ImdLatchedFault_set(io_faultLatch_getLatchedStatus(&imd_ok_latch));
    app_canTx_BMS_BspdLatchedFault_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch));

    // Wait for cell voltage and temperature measurements to settle. We expect to read back valid values from the
    // monitoring chips within 3 cycles
    const bool acc_fault           = app_accumulator_checkFaults();
    const bool settle_time_expired = app_timer_updateAndGetState(&cell_monitor_settle_timer) == TIMER_STATE_EXPIRED;

    if (acc_fault && settle_time_expired)
    {
        app_stateMachine_setNextState(app_faultState_get());
    }
}
