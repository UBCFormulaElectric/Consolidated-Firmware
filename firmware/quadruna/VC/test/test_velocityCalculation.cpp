#include "test_vcBaseStateMachineTest.h"

extern "C"
{
#include <assert.h>
#include "app_units.h"
#include "app_sbgEllipse.h"
#include "io_sbgEllipse.h"
}

class VelocityCalculationTest : public VCBaseTest
{
};

TEST_F(VelocityCalculationTest, velocity_calculation_when_rpms_same)
{
    TearDown();
    SetUp();

    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(10.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(10.0f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    float expectedVelocityKMH = 10.0f;

    ASSERT_NEAR(velocity.north, expectedVelocityKMH, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_when_rpms_zero)
{
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(0.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(0.0f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    ASSERT_NEAR(velocity.north, 0.0f, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_when_rpms_diff)
{
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(20.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(10.0f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    ASSERT_NEAR(velocity.north, 15.0f, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_when_one_rpm_neg)
{
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(4.0f));
    app_canRx_INVL_MotorSpeed_update(-MOTOR_KMH_TO_RPM(2.0f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    ASSERT_NEAR(velocity.north, 1.0f, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_when_rpms_neg)
{
    app_canRx_INVR_MotorSpeed_update(MOTOR_KMH_TO_RPM(5.0f));
    app_canRx_INVL_MotorSpeed_update(-MOTOR_KMH_TO_RPM(5.0f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    ASSERT_NEAR(velocity.north, -5.0f, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_precision)
{
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(59.9f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(60.1f));

    VelocityData velocity;

    app_sbgEllipse_calculateVelocity(&velocity);

    ASSERT_NEAR(velocity.north, 60.0f, 0.1f);
}