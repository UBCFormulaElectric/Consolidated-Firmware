#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_leds.hpp"
#include "fake_io_switches.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitors.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
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

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(&hb_monitor, true);

        app_stateMachine_init(app_mainState_get());

        app_heartbeatMonitor_clearFaults(&hb_monitor);
    }

    void TearDown() override
    {
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
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
};
