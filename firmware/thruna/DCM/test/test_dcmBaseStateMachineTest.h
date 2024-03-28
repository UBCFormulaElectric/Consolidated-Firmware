#pragma once

#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_buzzer.hpp"
#include "fake_io_sbgEllipse.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canUtils.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_utils.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "app_globals.h"
}

class DcmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_DCM_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);
        app_globals_init(&globals_config);

        // Default to starting the state machine in the `init` state
        app_stateMachine_init(app_initState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    std::vector<const State *> GetAllStates(void)
    {
        return std::vector<const State *>{ app_initState_get(), app_driveState_get() };
    }

    const BinaryLed brake_light = {};
    const Buzzer    buzzer      = {};

    // config to forward can functions to shared heartbeat
    // BMS rellies on DIM, FSM, and BMS
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                              [DCM_HEARTBEAT_BOARD] = false,
                                                              [PDM_HEARTBEAT_BOARD] = false,
                                                              [FSM_HEARTBEAT_BOARD] = true,
                                                              [DIM_HEARTBEAT_BOARD] = true };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                          [DCM_HEARTBEAT_BOARD] = NULL,
                                                          [PDM_HEARTBEAT_BOARD] = NULL,
                                                          [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_get,
                                                          [DIM_HEARTBEAT_BOARD] = &app_canRx_DIM_Heartbeat_get };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_update,
        [DIM_HEARTBEAT_BOARD] = &app_canRx_DIM_Heartbeat_update
    };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingBMSHeartbeat_set,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingFSMHeartbeat_set,
        [DIM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingDIMHeartbeat_set
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingBMSHeartbeat_get,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingFSMHeartbeat_get,
        [DIM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingDIMHeartbeat_get
    };

    const GlobalsConfig globals_config = {
        .brake_light = &brake_light,
    };
};