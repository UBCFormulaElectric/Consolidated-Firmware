#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"
#include "fake_io_led.hpp"
#include "fake_io_sbgEllipse.hpp"
#include "fake_io_pcm.hpp"
#include "fake_io_canLogging.hpp"

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
#include "app_faultCheck.h"
#include "app_regen.h"
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

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_init(true);
        // app_globals_init(&globals_config);

        app_efuse_init(efuse_enabled_can_setters, efuse_current_can_setters);

        // Default to starting the state machine in the `init` state
        app_stateMachine_init(app_initState_get());

        app_faultCheck_init();

        memset(&fake_sensor_data, 0U, sizeof(fake_sensor_data));

        fake_io_sbgEllipse_getImuAccelerations_returns(&fake_sensor_data.imu_data.acceleration);
        fake_io_sbgEllipse_getImuAngularVelocities_returns(&fake_sensor_data.imu_data.angular_velocity);
        fake_io_sbgEllipse_getEulerAngles_returns(&fake_sensor_data.euler_data.euler_angles);
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
        fake_io_pcm_set_reset();
        fake_io_sbgEllipse_getImuAccelerations_reset();
        fake_io_sbgEllipse_getImuAngularVelocities_reset();
        fake_io_sbgEllipse_getEulerAngles_reset();
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    void SetStateToDrive()
    {
        app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
        app_canRx_BMS_State_update(BMS_DRIVE_STATE);
        app_canRx_FSM_BrakeActuated_update(true);
        SetInitialState(app_driveState_get());
        app_heartbeatMonitor_clearFaults();
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

    SensorData fake_sensor_data;
};