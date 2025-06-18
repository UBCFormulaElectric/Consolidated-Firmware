#include <gtest/gtest.h>

#include "test_VCBase.hpp"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_driveHandling.h"
#include "app_canUtils.h"
}

class DriveHandlingTest : public VCBaseTest
{
};

TEST_F(DriveHandlingTest, DriveModeVanillaNotPowerLimiting_FullPower_LowRPM)
{
    // assumes full power
    uint32_t bms_avail_power_w = 80000;
    uint32_t inverter_rpm      = 1000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    ASSERT_TRUE(torqueOutputToMotors.front_left_torque == MAX_TORQUE_REQUEST_NM);
}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_FullPower_HighRPM)
{
    // Full power
    uint32_t bms_avail_power_w = 80000;
    // Higher RPM will enable power limiting
    uint32_t inverter_rpm      = 10000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    float expected_torque = (bms_avail_power_w * 0.8) / (4 * inverter_rpm * M_PI_F / 30.0f);

    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_right_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_right_torque, expected_torque);

}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_LessPower_LessRPM)
{
    // less power will enable power limiting
    uint32_t bms_avail_power_w = 70000;
    uint32_t inverter_rpm      = 1000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    ASSERT_TRUE(torqueOutputToMotors.front_left_torque == MAX_TORQUE_REQUEST_NM);
}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_LessPower_HighRPM)
{
    // less power will enable power limiting
    uint32_t bms_avail_power_w = 70000;
    // high rpm wll also enable power limiting
    uint32_t inverter_rpm      = 10000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(bms_avail_power_w);
    app_canRx_INVFR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVFL_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRR_ActualVelocity_update(inverter_rpm);
    app_canRx_INVRL_ActualVelocity_update(inverter_rpm);

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    ASSERT_TRUE(torqueOutputToMotors.front_left_torque < MAX_TORQUE_REQUEST_NM);
}
