#include "app_canUtils.h"
#include "test_VCBase.hpp"

extern "C"
{
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_driveHandling.h"
}

#define EFFICIENCY_ESTIMATE (0.80f)

class DriveHandlingTest : public VCBaseTest
{
};

TEST_F(DriveHandlingTest, DriveModeVanillaNotPowerLimiting)
{
    float       bms_avail_power_w = 80000 * EFFICIENCY_ESTIMATE;
    uint32_t    inverter_rpm      = 1000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(static_cast<uint32_t>(bms_avail_power_w));
    app_canRx_INVFR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVFL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    float torque_limit     = bms_avail_power_w / (4 * RPM_TO_RADS(static_cast<float>(inverter_rpm)));
    float torque_to_motors = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_EQ(torque_to_motors, MAX_TORQUE_REQUEST_NM);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_left_torque, torque_to_motors);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_right_torque, torque_to_motors);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_left_torque, torque_to_motors);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_right_torque, torque_to_motors);
}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_FullPower_HighRPM)
{
    // Full power
    float bms_avail_power_w = 80000 * EFFICIENCY_ESTIMATE;
    // Higher RPM will enable power limiting
    uint32_t    inverter_rpm = 10000;
    const float apps         = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(static_cast<uint32_t>(bms_avail_power_w));
    app_canRx_INVFR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVFL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    float torque_limit =
        (bms_avail_power_w * EFFICIENCY_ESTIMATE) / (4 * RPM_TO_RADS(static_cast<float>(inverter_rpm)));
    float expected_torque = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_right_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_right_torque, expected_torque);
}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_LessPower_LessRPM)
{
    // less power will enable power limiting
    float       bms_avail_power_w = 70000 * EFFICIENCY_ESTIMATE;
    uint32_t    inverter_rpm      = 1000;
    const float apps              = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(static_cast<uint32_t>(bms_avail_power_w));
    app_canRx_INVFR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVFL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    float torque_limit =
        (bms_avail_power_w * EFFICIENCY_ESTIMATE) / (4 * RPM_TO_RADS(static_cast<float>(inverter_rpm)));
    float expected_torque = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_right_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_right_torque, expected_torque);
}

TEST_F(DriveHandlingTest, DriveModeVanillaWithPowerLimiting_LessPower_HighRPM)
{
    // less power will enable power limiting
    float bms_avail_power_w = 70000 * EFFICIENCY_ESTIMATE;
    // high rpm wll also enable power limiting
    uint32_t    inverter_rpm = 10000;
    const float apps         = 1.0f;

    TorqueAllocationOutputs torqueOutputToMotors;

    app_canRx_BMS_AvailablePower_update(static_cast<uint32_t>(bms_avail_power_w));
    app_canRx_INVFR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVFL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRR_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));
    app_canRx_INVRL_ActualVelocity_update(static_cast<int32_t>(inverter_rpm));

    app_vanillaDrive_run(apps, &torqueOutputToMotors);

    float torque_limit =
        (bms_avail_power_w * EFFICIENCY_ESTIMATE) / (4 * RPM_TO_RADS(static_cast<float>(inverter_rpm)));
    float expected_torque = fminf(torque_limit, MAX_TORQUE_REQUEST_NM);

    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.front_right_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_left_torque, expected_torque);
    ASSERT_FLOAT_EQ(torqueOutputToMotors.rear_right_torque, expected_torque);
}
