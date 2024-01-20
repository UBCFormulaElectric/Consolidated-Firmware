#pragma once

#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_airs.hpp"
#include "fake_io_charger.hpp"
#include "fake_io_eeprom.hpp"
#include "fake_io_faultLatch.hpp"
#include "fake_io_imd.hpp"
#include "fake_io_ltc6813CellTemps.hpp"
#include "fake_io_ltc6813CellVoltages.hpp"
#include "fake_io_ltc6813Shared.hpp"
#include "fake_io_thermistors.hpp"
#include "fake_io_tractiveSystem.hpp"

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
#include "states/app_prechargeState.h"
#include "states/app_driveState.h"
#include "states/app_faultState.h"
#include "states/app_inverterOnState.h"
#include "states/app_balancingState.h"
#include "states/app_allStates.h"

#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_globals.h"
}

class BmsBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        app_inverterOnState_init();
        app_accumulator_init();
        app_thermistors_init();
        app_soc_init();
        app_globals_init(&globals_config);
        globals->hb_monitor = heartbeat_monitor;

        app_soc_resetSocCustomValue(100.0f);

        // Set initial voltages to nominal value
        fake_io_ltc6813CellVoltages_readVoltages_returns(true);
        fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(3.8f);

        // A temperature in [0.0, 60.0] degC to prevent other tests from entering the fault state
        fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(20.0f);
        fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(20.0f);

        // Disable charging
        App_CanRx_Debug_StartCharging_Update(false);
        fake_io_charger_hasFaulted_returns(false);
        fake_io_charger_isConnected_returns(false);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(NULL, app_initState_get());
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);

        fake_io_charger_hasFaulted_reset();
        fake_io_charger_isConnected_reset();
        fake_io_ltc6813CellTemps_getMinTempDegC_reset();
        fake_io_ltc6813CellTemps_getMaxTempDegC_reset();
        fake_io_ltc6813CellVoltages_getCellVoltage_reset();
        fake_io_tractiveSystem_getVoltage_reset();
        fake_io_airs_isNegativeClosed_reset();
        fake_io_airs_isPositiveClosed_reset();
        fake_io_imd_getFrequency_reset();
        fake_io_imd_getDutyCycle_reset();
        fake_io_faultLatch_getCurrentStatus_reset();
        fake_io_tractiveSystem_getCurrentHighResolution_reset();
        fake_io_tractiveSystem_getCurrentLowResolution_reset();
        fake_io_charger_enable_reset();
        fake_io_ltc6813CellTemps_getMinTempDegC_reset();
        fake_io_ltc6813CellTemps_getMaxTempDegC_reset();
        fake_io_airs_closePositive_reset();
        fake_io_faultLatch_setCurrentStatus_reset();
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(NULL, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{ app_initState_get(),     app_prechargeState_get(),
                                                  app_driveState_get(),    app_chargeState_get(),
                                                  app_faultState_get(),    app_inverterOnState_get(),
                                                  app_balancingState_get() };
    }

    // void CheckInRangeCanSignalsInGivenState(
    //     const State *state,
    //     float        min_value,
    //     float        max_value,
    //     float &      fake_value,
    //     float (*value_can_signal_getter)(const struct BmsCanTxInterface *),
    //     uint8_t (*out_of_range_can_signal_getter)(const struct BmsCanTxInterface *),
    //     uint8_t ok_choice,
    //     uint8_t underflow_choice,
    //     uint8_t overflow_choice)
    // {
    //     SetInitialState(state);

    //     // Normal range
    //     fake_value = (min_value + max_value) / 2;
    //     LetTimePass(state_machine, 1000);
    //     ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
    //     ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

    //     // Underflow range
    //     fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
    //     LetTimePass(state_machine, 1000);
    //     ASSERT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

    //     // Overflow range
    //     fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
    //     LetTimePass(state_machine, 1000);
    //     ASSERT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
    // }

    // void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    // {
    //     struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    //     struct Clock *   clock = App_BmsWorld_GetClock(world);
    //     App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    //     App_Timer_SetCurrentTimeMS(current_time_ms);
    // }

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;

    const Charger              charger_config     = {};
    const ThermistorsConfig    thermistors_config = {};
    const AirsConfig           airs_config        = {};
    const TractiveSystemConfig ts_config          = {};
    const FaultLatch           bms_ok_latch       = {};
    const FaultLatch           imd_ok_latch       = {};
    const FaultLatch           bspd_ok_latch      = {};

    const GlobalsConfig globals_config = {
        .bms_ok_latch  = &bms_ok_latch,
        .imd_ok_latch  = &imd_ok_latch,
        .bspd_ok_latch = &bspd_ok_latch,
    };
};