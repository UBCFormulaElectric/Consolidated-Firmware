#pragma once
#include <gtest/gtest.h>
#include "fake_io_time.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "app_mainState.h"
#include "app_heartbeatMonitor.h"
#include "app_globals.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class FsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        // TODO: replace first NULL argument with timer
        app_heartbeatMonitor_init(
            HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
            &app_canTx_FSM_Heartbeat_set, heartbeatFaultSetters, heartbeatFaultGetters);

        app_stateMachine_init(app_mainState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    // config to forward can functions to shared heartbeat
    // FSM rellies on TBD
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = false,
                                                              [DCM_HEARTBEAT_BOARD] = false,
                                                              [PDM_HEARTBEAT_BOARD] = false,
                                                              [FSM_HEARTBEAT_BOARD] = false,
                                                              [DIM_HEARTBEAT_BOARD] = false };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = false,
                                                          [DCM_HEARTBEAT_BOARD] = false,
                                                          [PDM_HEARTBEAT_BOARD] = false,
                                                          [FSM_HEARTBEAT_BOARD] = false,
                                                          [DIM_HEARTBEAT_BOARD] = false };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                               [DCM_HEARTBEAT_BOARD] = NULL,
                                                               [PDM_HEARTBEAT_BOARD] = NULL,
                                                               [FSM_HEARTBEAT_BOARD] = NULL,
                                                               [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                                   [DCM_HEARTBEAT_BOARD] = NULL,
                                                                   [PDM_HEARTBEAT_BOARD] = NULL,
                                                                   [FSM_HEARTBEAT_BOARD] = NULL,
                                                                   [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = false,
                                                               [DCM_HEARTBEAT_BOARD] = false,
                                                               [PDM_HEARTBEAT_BOARD] = false,
                                                               [FSM_HEARTBEAT_BOARD] = false,
                                                               [DIM_HEARTBEAT_BOARD] = false };
};
