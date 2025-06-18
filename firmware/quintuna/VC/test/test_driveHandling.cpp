#include "app_canUtils.h"
#include "test_VCBase.hpp"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_pcm.h"
#include "app_vehicleDynamicsConstants.h"
}

class DriveHandlingTest : public VCBaseTest
{
};

TEST_F(DriveHandlingTest, DriveModeVanillaNotPowerLimiting)
{
    uint32_t bms_avail_power_w = 80000;
    uint32_t inverter_rpm      = 1000;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    float torque_limit = (float)(bms_avail_power_w / (4 * inverter_rpm));
    float torque_to_motors = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_TRUE(torque_to_motors == MAX_TORQUE_REQUEST_NM);
}

TEST_F(DriveHandlingTest, DriveModeVanillaNotPowerLimiting)
{
    uint32_t bms_avail_power_w = 80000;
    uint32_t inverter_rpm      = 1000;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    float torque_limit = (float)(bms_avail_power_w / (4 * RPM_TO_RADS(inverter_rpm)));
    float torque_to_motors = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_TRUE(torque_to_motors == MAX_TORQUE_REQUEST_NM);
}