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
#include "app_sevenSegDisplays.h"
#include "app_avgPower.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_globals.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class DimBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
            &app_canTx_DIM_Heartbeat_set, heartbeatFaultSetters, heartbeatFaultGetters);
        app_avgPower_init();
        app_sevenSegDisplays_init();
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
        fake_io_led_enable_reset();
        fake_io_rgbLed_enable_reset();
        fake_io_rgbLed_disable_reset();
        fake_io_switch_isClosed_reset();
    }

    const BinaryLed       imd_led           = {};
    const BinaryLed       bspd_led          = {};
    const BinaryLed       shdn_led          = {};
    const BinaryLed       drive_led         = {};
    const Switch          start_switch      = {};
    const Switch          aux_switch        = {};
    const RgbLed          bms_status_led    = {};
    const RgbLed          dcm_status_led    = {};
    const RgbLed          fsm_status_led    = {};
    const RgbLed          pdm_status_led    = {};
    const RgbLed          dim_status_led    = {};
    const SevenSegsConfig seven_segs_config = {};

    // config to forward can functions to shared heartbeat
    // DIM rellies on all boards but itself
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                              [DCM_HEARTBEAT_BOARD] = true,
                                                              [PDM_HEARTBEAT_BOARD] = true,
                                                              [FSM_HEARTBEAT_BOARD] = true,
                                                              [DIM_HEARTBEAT_BOARD] = false };

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                          [DCM_HEARTBEAT_BOARD] = &app_canRx_DCM_Heartbeat_get,
                                                          [PDM_HEARTBEAT_BOARD] = &app_canRx_PDM_Heartbeat_get,
                                                          [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_get,
                                                          [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                               [DCM_HEARTBEAT_BOARD] = &app_canRx_DCM_Heartbeat_update,
                                                               [PDM_HEARTBEAT_BOARD] = &app_canRx_PDM_Heartbeat_update,
                                                               [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_update,
                                                               [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatFaultSetters - broadcast heartbeat faults over CAN
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingBMSHeartbeat_set,
        [DCM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingDCMHeartbeat_set,
        [PDM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingPDMHeartbeat_set,
        [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingFSMHeartbeat_set,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingBMSHeartbeat_get,
        [DCM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingDCMHeartbeat_get,
        [PDM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingPDMHeartbeat_get,
        [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DIM_Fault_MissingFSMHeartbeat_get,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    const GlobalsConfig globals_config = {
        .imd_led        = &imd_led,
        .bspd_led       = &bspd_led,
        .shdn_led       = &shdn_led,
        .drive_led      = &drive_led,
        .start_switch   = &start_switch,
        .aux_switch     = &aux_switch,
        .bms_status_led = &bms_status_led,
        .dcm_status_led = &dcm_status_led,
        .fsm_status_led = &fsm_status_led,
        .pdm_status_led = &pdm_status_led,
        .dim_status_led = &dim_status_led,
    };
};
