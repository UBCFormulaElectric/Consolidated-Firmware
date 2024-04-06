#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_coolant.hpp"
#include "fake_io_loadCell.hpp"
#include "fake_io_suspension.hpp"

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
#include "app_coolant.h"
#include "app_globals.h"
#include "app_loadCell.h"
#include "app_suspension.h"
}
// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.
class RsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_coolant_init();
        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_RSM_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);
        app_stateMachine_init(app_mainState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        fake_io_coolant_getFlowRate_reset();
        fake_io_coolant_checkIfFlowMeterActive_reset();
        fake_io_coolant_getTemperatureA_reset();
        fake_io_coolant_getTemperatureB_reset();
        fake_io_coolant_getPressureA_reset();
        fake_io_coolant_getPressureB_reset();

        fake_io_loadCell_getMechanicalLoad3_reset();
        fake_io_loadCell_getMechanicalLoad4_reset();
        fake_io_loadCell_sensor3OCSC_reset();
        fake_io_loadCell_sensor4OCSC_reset();

        fake_io_suspension_getRearLeftTravel_reset();
        fake_io_suspension_getRearRightTravel_reset();
        fake_io_suspension_leftSensorOCSC_reset();
        fake_io_suspension_rightSensorOCSC_reset();
    }

    // config for heartbeat monitor (can funcs and flags)
    // RSM rellies on BMS and FSM
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
        [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = app_canRx_BMS_Heartbeat_get, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = app_canRx_FSM_Heartbeat_get, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
                                                               [VC_HEARTBEAT_BOARD]   = NULL,
                                                               [RSM_HEARTBEAT_BOARD]  = NULL,
                                                               [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
                                                               [DIM_HEARTBEAT_BOARD]  = NULL,
                                                               [CRIT_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_set,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL,
        [FSM_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_set,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_get,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL,
        [FSM_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_get,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };
};