#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

// TODO: include fakes

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class FsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_FSM_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // TODO: reset fakes
    }

    // config for heartbeat monitor (can funcs and flags)
    // TODO: fill out these configs
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = false, [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
        [FSM_HEARTBEAT_BOARD] = false, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = NULL, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = NULL, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = NULL, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = NULL, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
    };
};
