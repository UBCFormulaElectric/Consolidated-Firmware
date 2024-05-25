#pragma once

#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_airs.hpp"
#include "fake_io_charger.hpp"
#include "fake_io_sd.hpp"
#include "fake_io_faultLatch.hpp"
#include "fake_io_imd.hpp"
#include "fake_io_ltc6813CellTemps.hpp"
#include "fake_io_ltc6813CellVoltages.hpp"
#include "fake_io_ltc6813Shared.hpp"
#include "fake_io_thermistors.hpp"
#include "fake_io_tractiveSystem.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canUtils.h"
#include "app_stateMachine.h"
#include "app_utils.h"
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

        app_canTx_init();
        app_canRx_init();

        app_inverterOnState_init();
        app_accumulator_init();
        app_thermistors_init();
        app_soc_init();
        app_globals_init(&globals_config);

        app_soc_resetSocCustomValue(100.0f);

        // Set initial voltages to nominal value
        fake_io_ltc6813CellVoltages_readVoltages_returns(true);
        fake_io_ltc6813CellVoltages_owcReadVoltages_returnsForAnyArgs(true);
        fake_io_ltc6813CellVoltages_getCellVoltage_returnsForAnyArgs(3.8f);

        // A temperature in [0.0, 60.0] degC to prevent other tests from entering the fault state
        fake_io_ltc6813CellTemps_getMinTempDegC_returnsForAnyArgs(20.0f);
        fake_io_ltc6813CellTemps_getMaxTempDegC_returnsForAnyArgs(20.0f);

        // Disable charging
        app_canRx_Debug_StartCharging_update(false);
        fake_io_charger_hasFaulted_returns(false);
        fake_io_charger_isConnected_returns(false);

        // Default to starting the state machine in the `init` state
        app_stateMachine_init(app_initState_get());
    }

    void TearDown() override
    {
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

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    std::vector<const State *> GetAllStates(void)
    {
        return std::vector<const State *>{ app_initState_get(),     app_prechargeState_get(), app_driveState_get(),
                                           app_chargeState_get(),   app_faultState_get(),     app_inverterOnState_get(),
                                           app_balancingState_get() };
    }

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