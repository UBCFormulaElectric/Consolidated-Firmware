/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 * 
 * 
 * 
*/

#include "Test_Dcm.h"

extern "C"
{
#include "App_PowerLimiting.h"
#include "App_TorqueVectoring.h"
#include "App_TorqueVectoringConstants.h"
#include "math.h"
}

/**
 * Define SetUp and TearDown to be called before every TEST_F call
*/
class PowerLimitingTest : public testing::Test
{
    protected: 
        //TODO: Find out if this is necessary if no IO functions are called.
        void SetUp() override
        {
            return;
        }
        void TearDown() override
        {
            return;
        }
};


// Test1: Limit on max motor temps
PowerLimiting_Inputs* power_limiting_inputs_test1 = {
    100.0,
    100.0,
    POWER_LIMIT_CAR_kW,
    1.00
};

TEST_F(PowerLimitingTest, test_motor_temp_limits_power)
{
    float expected_power_limit_test1 = POWER_LIMIT_CAR_kW - (fmaxf(power_limiting_inputs_test1->left_motor_temp_C, power_limiting_inputs_test1->right_motor_temp_C)- MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float actual_power_limit_test1 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test1);
    ASSERT_FLOAT_EQ(actual_power_limit_test1, expected_power_limit_test1);
}


// Test2: Limit on BMS available power
PowerLimiting_Inputs* power_limiting_inputs_test2 = {
    10.0,
    10.0,
    50.0,
    1.00
};

TEST_F(PowerLimitingTest, test_bms_limits_power)
{
    float expected_power_limit_test2 = 50.0;
    float actual_power_limit_test2 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test2);
    ASSERT_FLOAT_EQ(actual_power_limit_test2, expected_power_limit_test2);
}


// Test3: Limit on pedal percentage
PowerLimiting_Inputs* power_limiting_inputs_test3 = {
    10.0,
    10.0,
    POWER_LIMIT_CAR_kW,
    0.50
};

TEST_F(PowerLimitingTest, test_pedal_limits_power)
{
    float expected_power_limit_test3 = POWER_LIMIT_CAR_kW * power_limiting_inputs_test3->accelerator_pedal_percent;
    ASSERT_FLOAT_EQ(App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test3), expected_power_limit_test3);
}


// Test4 Pedal not pressed
PowerLimiting_Inputs* power_limiting_inputs_test4 = {
    10.0,
    10.0,
    POWER_LIMIT_CAR_kW,
    0.00
}

TEST_F(PowerLimitingTest, test_pedal_not_pressed)
{
    ASSERT_FLOAT_EQ(App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test4), 0.0f);
}