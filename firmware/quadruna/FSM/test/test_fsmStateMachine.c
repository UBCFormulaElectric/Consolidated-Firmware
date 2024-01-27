#include <gtest/gtest.h>
#include "Test_Utils.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/app_initState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
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

        // config to forward can functions to shared heartbeat
        // FSM rellies on TBD
        bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = false,
                                                                  [DCM_HEARTBEAT_BOARD] = false,
                                                                  [PDM_HEARTBEAT_BOARD] = false,
                                                                  [FSM_HEARTBEAT_BOARD] = false,
                                                                  [DIM_HEARTBEAT_BOARD] = false };

        // heartbeatGetters - get heartbeat signals from other boards
        bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                              [DCM_HEARTBEAT_BOARD] = NULL,
                                                              [PDM_HEARTBEAT_BOARD] = NULL,
                                                              [FSM_HEARTBEAT_BOARD] = NULL,
                                                              [DIM_HEARTBEAT_BOARD] = NULL };

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
        bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] =
                                                                       &App_CanAlerts_FSM_Fault_MissingBMSHeartbeat_Get,
                                                                   [DCM_HEARTBEAT_BOARD] = NULL,
                                                                   [PDM_HEARTBEAT_BOARD] = NULL,
                                                                   [FSM_HEARTBEAT_BOARD] = NULL,
                                                                   [DIM_HEARTBEAT_BOARD] = NULL };

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters,
            heartbeatUpdaters, &App_CanTx_FSM_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);
        state_machine = App_SharedStateMachine_Create(NULL, app_driveState_get());

        globals->heartbeat_monitor = heartbeat_monitor;
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
    }

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
};
