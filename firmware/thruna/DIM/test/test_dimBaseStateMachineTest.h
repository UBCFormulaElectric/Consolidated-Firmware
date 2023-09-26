#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_rgbLed.hpp"
#include "fake_io_switch.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "app_sevenSegDisplays.h"
#include "App_SharedRgbLedSequence.h"
#include "app_avgPower.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/app_driveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
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

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);
        state_machine = App_SharedStateMachine_Create(NULL, app_driveState_get());

        app_avgPower_init();
        app_sevenSegDisplays_init();
        app_globals_init(&globals_config);
        globals->heartbeat_monitor = heartbeat_monitor;
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);

        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_led_enable_reset();
        fake_io_rgbLed_enable_reset();
        fake_io_rgbLed_disable_reset();
        fake_io_switch_isClosed_reset();
    }

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;

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