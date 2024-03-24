#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"
#include "fake_io_led.hpp"
#include "fake_io_buzzer.hpp"
#include "fake_io_sbgEllipse.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "states/app_inverterOnState.h"
#include "app_powerManager.h"
#include "app_efuse.h"
#include "app_globals.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class VcBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_VC_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);
        app_globals_init(&globals_config);

        app_efuse_init(efuse_enabled_can_setters, efuse_current_can_setters);

        // Default to starting the state machine in the `init` state
        app_stateMachine_init(app_initState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasBoostControllerFault_reset();
        fake_io_lowVoltageBattery_getBatVoltage_reset();
        fake_io_lowVoltageBattery_getBoostVoltage_reset();
        fake_io_lowVoltageBattery_getAccVoltage_reset();
        fake_io_efuse_setChannel_reset();
        fake_io_efuse_isChannelEnabled_reset();
        fake_io_efuse_getChannelCurrent_reset();
        fake_io_efuse_standbyReset_reset();
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    // configs for efuse messages over can
    void (*efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
        [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnStatus_set,
        [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvStatus_set,
        [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpStatus_set,
        [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxStatus_set,
        [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRStatus_set,
        [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLStatus_set,
        [EFUSE_CHANNEL_TELEM]  = NULL,
        [EFUSE_CHANNEL_BUZZER] = NULL,
    };

    void (*efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
        [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnCurrent_set,
        [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvCurrent_set,
        [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpCurrent_set,
        [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxCurrent_set,
        [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRCurrent_set,
        [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLCurrent_set,
        [EFUSE_CHANNEL_TELEM]  = NULL,
        [EFUSE_CHANNEL_BUZZER] = NULL,
    };
    std::vector<const State *> GetAllStates(void)
    {
        return std::vector<const State *>{ app_initState_get(), app_driveState_get() };
    }

    const BinaryLed brake_light = {};
    const Buzzer    buzzer      = {};

    // config for heartbeat monitor (can funcs and flags)
    // VC relies on FSM, RSM, BMS, CRIT
    // TODO: add RSM to config when boards are ready, also add vitals to canRx json
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = false, [RSM_HEARTBEAT_BOARD] = true,
        [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = true, [CRIT_HEARTBEAT_BOARD] = true
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
                                                          [VC_HEARTBEAT_BOARD]   = NULL,
                                                          [RSM_HEARTBEAT_BOARD]  = NULL, // app_canRx_RSM_Heartbeat_get
                                                          [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_get,
                                                          [DIM_HEARTBEAT_BOARD]  = NULL,
                                                          [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_get };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL, // app_canRx_RSM_Heartbeat_update
        [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_update
    };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_set,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL, // app_canAlerts_VC_Fault_MissingRSMHeartbeat_set
        [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_set,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_set
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_get,
        [VC_HEARTBEAT_BOARD]   = NULL,
        [RSM_HEARTBEAT_BOARD]  = NULL, // app_canAlerts_VC_Fault_MissingRSMHeartbeat_get
        [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_get,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_get
    };

    const GlobalsConfig globals_config = {
        .brake_light = &brake_light,
        .buzzer      = &buzzer,
    };
};