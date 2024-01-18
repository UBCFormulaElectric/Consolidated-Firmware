#pragma once

#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_buzzer.hpp"
#include "fake_io_sbgEllipse.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_CanUtils.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "configs/App_HeartbeatMonitorConfig.h"
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

        App_CanTx_Init();
        App_CanRx_Init();

        hb_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        app_globals_init(&globals_config);
        globals->hb_monitor = hb_monitor;

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(NULL, app_initState_get());
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(hb_monitor, App_SharedHeartbeatMonitor_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(NULL, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{ app_initState_get(), app_driveState_get() };
    }

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *hb_monitor;

    const BinaryLed brake_light = {};
    const Buzzer    buzzer      = {};

    const GlobalsConfig globals_config = {
        .brake_light = &brake_light,
        .buzzer      = &buzzer,
    };
};