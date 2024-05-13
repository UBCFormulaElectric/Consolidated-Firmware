#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

// #include "fake_io_led.hpp"
// #include "fake_io_rgbLed.hpp"
// #include "fake_io_switch.hpp"
#include "fake_io_leds.hpp"
#include "fake_io_switches.hpp"

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
#include "app_globals.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class CritBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_CRIT_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);
        app_globals_init(&globals_config);
        app_stateMachine_init(app_mainState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_led_init_reset();
        fake_io_led_imd_set_reset();
        fake_io_led_bspd_set_reset();
        fake_io_led_ams_set_reset();
        fake_io_led_start_set_reset();
        fake_io_led_regen_set_reset();
        fake_io_led_torquevec_set_reset();
        fake_io_led_shutdown_set_reset();
        fake_io_led_bms_status_set_reset();
        fake_io_led_fsm_status_set_reset();
        fake_io_led_vc_status_set_reset();
        fake_io_led_aux_status_set_reset();
        fake_io_led_crit_status_set_reset();
        fake_io_led_rsm_status_set_reset();
        fake_io_switches_init_reset();
        fake_io_switches_start_get_reset();
        fake_io_switches_regen_get_reset();
        fake_io_switches_torquevec_get_reset();
    }
    // dummy used to initialize shdn_sen_pin
    const DriveMode drive_mode = {};

    // config to forward can functions to shared heartbeat
    // DIM rellies on all boards but itself
    const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = true,   [RSM_HEARTBEAT_BOARD] = true,
        [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = &app_canRx_BMS_Heartbeat_get,
                                                                [VC_HEARTBEAT_BOARD]   = &app_canRx_VC_Heartbeat_get,
                                                                [RSM_HEARTBEAT_BOARD]  = &app_canRx_RSM_Heartbeat_get,
                                                                [FSM_HEARTBEAT_BOARD]  = &app_canRx_FSM_Heartbeat_get,
                                                                [DIM_HEARTBEAT_BOARD]  = nullptr,
                                                                [CRIT_HEARTBEAT_BOARD] = nullptr };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = &app_canRx_BMS_Heartbeat_update,
        [VC_HEARTBEAT_BOARD]   = &app_canRx_VC_Heartbeat_update,
        [RSM_HEARTBEAT_BOARD]  = &app_canRx_RSM_Heartbeat_update,
        [FSM_HEARTBEAT_BOARD]  = &app_canRx_FSM_Heartbeat_update,
        [DIM_HEARTBEAT_BOARD]  = nullptr,
        [CRIT_HEARTBEAT_BOARD] = nullptr
    };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set,
        [VC_HEARTBEAT_BOARD]   = &app_canAlerts_CRIT_Fault_MissingVCHeartbeat_set,
        [RSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set,
        [FSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set,
        [DIM_HEARTBEAT_BOARD]  = nullptr,
        [CRIT_HEARTBEAT_BOARD] = nullptr
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get,
        [VC_HEARTBEAT_BOARD]   = &app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get,
        [RSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get,
        [FSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get,
        [DIM_HEARTBEAT_BOARD]  = nullptr,
        [CRIT_HEARTBEAT_BOARD] = nullptr
    };

    const GlobalsConfig globals_config = {
        .drive_mode = &drive_mode,
    };
};
