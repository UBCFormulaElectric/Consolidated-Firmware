#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_coolant.hpp"
#include "fake_io_steering.hpp"
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
#include "app_coolant.h"
#include "app_steering.h"
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
        app_coolant_init();
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
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_apps_getPrimary_reset();
        fake_io_apps_getSecondary_reset();
        fake_io_apps_isPrimaryOCSC_reset();
        fake_io_apps_isSecondaryOCSC_reset();
        fake_io_brake_isActuated_reset();
        fake_io_brake_getPedalPercentTravel_reset();
        fake_io_brake_pedalSensorOCSC_reset();
        fake_io_coolant_getFlowRate_reset();
        fake_io_steering_sensorOCSC_reset();
    }

    // config to forward can functions to shared heartbeat
    // FSM rellies on BMS
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                              [DCM_HEARTBEAT_BOARD] = false,
                                                              [PDM_HEARTBEAT_BOARD] = false,
                                                              [FSM_HEARTBEAT_BOARD] = false,
                                                              [DIM_HEARTBEAT_BOARD] = false };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                          [DCM_HEARTBEAT_BOARD] = NULL,
                                                          [PDM_HEARTBEAT_BOARD] = NULL,
                                                          [FSM_HEARTBEAT_BOARD] = NULL,
                                                          [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                               [DCM_HEARTBEAT_BOARD] = NULL,
                                                               [PDM_HEARTBEAT_BOARD] = NULL,
                                                               [FSM_HEARTBEAT_BOARD] = NULL,
                                                               [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_FSM_Fault_MissingBMSHeartbeat_set,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL,
        [DIM_HEARTBEAT_BOARD] = NULL
    };
};
