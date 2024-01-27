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
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_utils.h"
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

        heartbeat_monitor = app_heartbeatMonitor_init(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters,
            heartbeatUpdaters, &App_CanTx_BMS_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);

        app_inverterOnState_init();
        app_accumulator_init();
        app_thermistors_init();
        app_soc_init();
        app_globals_init(&globals_config);
        = heartbeat_monitor;

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
        = app_stateMachine_init(NULL, app_initState_get());
    }

    void TearDown() override
    {
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

    void SetInitialState(const State *const initial_state)
    {
        = app_stateMachine_init(NULL, initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    std::vector<const State *> GetAllStates(void)
    {
        return std::vector<const State *>{ app_initState_get(),     app_prechargeState_get(), app_driveState_get(),
                                           app_chargeState_get(),   app_faultState_get(),     app_inverterOnState_get(),
                                           app_balancingState_get() };
    }

    StateMachine *;
    HeartbeatMonitor *heartbeat_monitor;

    const Charger              charger_config     = {};
    const ThermistorsConfig    thermistors_config = {};
    const AirsConfig           airs_config        = {};
    const TractiveSystemConfig ts_config          = {};
    const FaultLatch           bms_ok_latch       = {};
    const FaultLatch           imd_ok_latch       = {};
    const FaultLatch           bspd_ok_latch      = {};

    // config to forward can functions to shared heartbeat
    // BMS rellies on DCM, PDM, and FSM
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = false,
                                                              [DCM_HEARTBEAT_BOARD] = true,
                                                              [PDM_HEARTBEAT_BOARD] = true,
                                                              [FSM_HEARTBEAT_BOARD] = true,
                                                              [DIM_HEARTBEAT_BOARD] = false };
    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                          [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Get,
                                                          [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Get,
                                                          [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Get,
                                                          [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                               [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Update,
                                                               [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Update,
                                                               [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Update,
                                                               [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = NULL,
        [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingDCMHeartbeat_Set,
        [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingPDMHeartbeat_Set,
        [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingFSMHeartbeat_Set,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = NULL,
        [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingDCMHeartbeat_Get,
        [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingPDMHeartbeat_Get,
        [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingFSMHeartbeat_Get,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    const GlobalsConfig globals_config = {
        .bms_ok_latch  = &bms_ok_latch,
        .imd_ok_latch  = &imd_ok_latch,
        .bspd_ok_latch = &bspd_ok_latch,
    };
};