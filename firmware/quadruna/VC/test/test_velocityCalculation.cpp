#include "test_vcBaseStateMachineTest.h"

extern "C"
{
#include <assert.h>
#include "app_units.h"
#include "app_sbgEllipse.h"
#include "io_sbgEllipse.h"
}

class VelocityCalculationTest : public VcBaseStateMachineTest
{
};

TEST_F(VelocityCalculationTest, velocity_calculation_set)
{
    TearDown();
    SetUp();

    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(10.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(10.0f));

    VelocityData velocity = app_sbgEllipse_calculateVelocity();

    float expectedVelocityKMH = 10.0f;

    ASSERT_NEAR(velocity.north, expectedVelocityKMH, 0.1f);
}

TEST_F(VelocityCalculationTest, velocity_calculation_when_rpms_zero) 
{
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(0.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(0.0f));

    VelocityData velocity = app_sbgEllipse_calculateVelocity();

    ASSERT_NEAR(velocity.north, 0.0f, 0.1f);
}