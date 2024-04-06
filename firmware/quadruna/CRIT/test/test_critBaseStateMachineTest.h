#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_led.hpp"
#include "fake_io_rgbLed.hpp"
#include "fake_io_switch.hpp"

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
        // app_globals_init(&globals_config);
        app_stateMachine_init(app_mainState_get());

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_led_enable_reset();
        fake_io_rgbLed_enable_reset();
        fake_io_rgbLed_disable_reset();
        fake_io_switch_isClosed_reset();
    }

    const BinaryLed imd_led          = {};
    const BinaryLed bspd_led         = {};
    const BinaryLed ams_led          = {};
    const BinaryLed shdn_led         = {};
    const BinaryLed start_led        = {};
    const Switch    start_switch     = {};
    const BinaryLed regen_led        = {};
    const Switch    regen_switch     = {};
    const BinaryLed torquevec_led    = {};
    const Switch    torquevec_switch = {};
    const RgbLed    aux_status_led   = {};
    const RgbLed    bms_status_led   = {};
    const RgbLed    crit_status_led  = {};
    const RgbLed    fsm_status_led   = {};
    const RgbLed    rsm_status_led   = {};
    const RgbLed    vc_status_led    = {};
    // dummy used to initialize shdn_sen_pin
    const ShutdownSensor shdn_sen     = {};

    // config to forward can functions to shared heartbeat
    // DIM rellies on all boards but itself
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = true,   [RSM_HEARTBEAT_BOARD] = true,
        [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
    };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = &app_canRx_BMS_Heartbeat_get,
                                                          [VC_HEARTBEAT_BOARD]   = &app_canRx_VC_Heartbeat_get,
                                                          [RSM_HEARTBEAT_BOARD]  = &app_canRx_RSM_Heartbeat_get,
                                                          [FSM_HEARTBEAT_BOARD]  = &app_canRx_FSM_Heartbeat_get,
                                                          [DIM_HEARTBEAT_BOARD]  = NULL,
                                                          [CRIT_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = &app_canRx_BMS_Heartbeat_update,
                                                               [VC_HEARTBEAT_BOARD]   = &app_canRx_VC_Heartbeat_update,
                                                               [RSM_HEARTBEAT_BOARD]  = &app_canRx_RSM_Heartbeat_update,
                                                               [FSM_HEARTBEAT_BOARD]  = &app_canRx_FSM_Heartbeat_update,
                                                               [DIM_HEARTBEAT_BOARD]  = NULL,
                                                               [CRIT_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set,
        [VC_HEARTBEAT_BOARD]   = &app_canAlerts_CRIT_Fault_MissingVCHeartbeat_set,
        [RSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set,
        [FSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get,
        [VC_HEARTBEAT_BOARD]   = &app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get,
        [RSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get,
        [FSM_HEARTBEAT_BOARD]  = &app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get,
        [DIM_HEARTBEAT_BOARD]  = NULL,
        [CRIT_HEARTBEAT_BOARD] = NULL
    };

    const GlobalsConfig globals_config = {
        .imd_led          = &imd_led,
        .bspd_led         = &bspd_led,
        .ams_led          = &ams_led,
        .shdn_led         = &shdn_led,
        .start_led        = &start_led,
        .start_switch     = &start_switch,
        .regen_led        = &regen_led,
        .regen_switch     = &regen_switch,
        .torquevec_led    = &torquevec_led,
        .torquevec_switch = &torquevec_switch,
        .aux_status_led   = &aux_status_led,
        .bms_status_led   = &bms_status_led,
        .crit_status_led  = &crit_status_led,
        .fsm_status_led   = &fsm_status_led,
        .rsm_status_led   = &rsm_status_led,
        .vc_status_led    = &vc_status_led,
        .shdn_sen     = &shdn_sen
    };
};
