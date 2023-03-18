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
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
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

TEST_F(PowerLimitingTest, motor_temps_limit_power)
{
    struct PowerLimiting_Inputs* power_limiting_inputs_test1;
    *power_limiting_inputs_test1 = {100.0, 100.0, POWER_LIMIT_CAR_kW, 1.00}; 

    float expected_power_limit_test1 = POWER_LIMIT_CAR_kW - (fmaxf(power_limiting_inputs_test1->left_motor_temp_C, power_limiting_inputs_test1->right_motor_temp_C)- MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float actual_power_limit_test1 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test1);
    ASSERT_FLOAT_EQ(actual_power_limit_test1, expected_power_limit_test1);
}


// Test2: Limit on BMS available power

TEST_F(PowerLimitingTest, bms_limits_power)
{
    struct PowerLimiting_Inputs* power_limiting_inputs_test2;
    *power_limiting_inputs_test2 = {10.0, 10.0, 50.0, 1.00};
    float expected_power_limit_test2 = 50.0;
    float actual_power_limit_test2 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test2);
    ASSERT_FLOAT_EQ(actual_power_limit_test2, expected_power_limit_test2);
}


// Test3: Limit on pedal percentage

TEST_F(PowerLimitingTest, pedal_limits_power)
{
    struct PowerLimiting_Inputs* power_limiting_inputs_test3;
    *power_limiting_inputs_test3 = {10.0, 10.0, POWER_LIMIT_CAR_kW, 0.50};
    float expected_power_limit_test3 = POWER_LIMIT_CAR_kW * power_limiting_inputs_test3->accelerator_pedal_percent;
    float actual_power_limit_test3 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test3);
    ASSERT_FLOAT_EQ(actual_power_limit_test3, expected_power_limit_test3);
}


// Test4 Pedal not pressed

TEST_F(PowerLimitingTest, no_power_when_pedal_not_pressed)
{
    struct PowerLimiting_Inputs* power_limiting_inputs_test4;
    *power_limiting_inputs_test4 = {10.0, 10.0, POWER_LIMIT_CAR_kW, 0.00};
    float expected_power_limit_test4 = 0.0;
    float actual_power_limit_test4 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test4);
    ASSERT_FLOAT_EQ(actual_power_limit_test4, expected_power_limit_test4);
}