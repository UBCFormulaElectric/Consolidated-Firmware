#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_steering.h"
#include "app_suspension.h"
#include "app_wheels.h"
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

        app_apps_init();
        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_FSM_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);
        app_stateMachine_init(app_mainState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // Reset fakes
        fake_io_apps_init_reset();
        fake_io_apps_getPrimary_reset();
        fake_io_apps_isPrimaryOCSC_reset();
        fake_io_apps_getSecondary_reset();
        fake_io_apps_isSecondaryOCSC_reset();

        fake_io_brake_isActuated_reset();
        fake_io_brake_getFrontPressurePsi_reset();
        fake_io_brake_frontPressureSensorOCSC_reset();
        fake_io_brake_getRearPressurePsi_reset();
        fake_io_brake_rearPressureSensorOCSC_reset();

        fake_io_steering_getAngleDegrees_reset();
        fake_io_steering_sensorOCSC_reset();

        fake_io_suspension_getLeftTravel_reset();
        fake_io_suspension_getRightTravel_reset();
        fake_io_suspension_leftSensorOCSC_reset();
        fake_io_suspension_rightSensorOCSC_reset();

        fake_io_wheels_getLeftSpeedKph_reset();
        fake_io_wheels_getRightSpeedKph_reset();
        fake_io_wheels_checkIfLeftSensorActive_reset();
        fake_io_wheels_checkIfRightSensorActive_reset();
    }

    // config for heartbeat monitor (can funcs and flags)
    // FSM rellies on BMS
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true,  [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
        [FSM_HEARTBEAT_BOARD] = false, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
                                                          [VC_HEARTBEAT_BOARD]   = NULL,
                                                          [RSM_HEARTBEAT_BOARD]  = NULL,
                                                          [FSM_HEARTBEAT_BOARD]  = NULL,
                                                          [DIM_HEARTBEAT_BOARD]  = NULL,
                                                          [CRIT_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
                                                               [VC_HEARTBEAT_BOARD]   = NULL,
                                                               [RSM_HEARTBEAT_BOARD]  = NULL,
                                                               [FSM_HEARTBEAT_BOARD]  = NULL,
                                                               [DIM_HEARTBEAT_BOARD]  = NULL,
                                                               [CRIT_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_set,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL,
        [FSM_HEARTBEAT_BOARD]  = NULL,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL,
        [FSM_HEARTBEAT_BOARD]  = NULL,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };
};
