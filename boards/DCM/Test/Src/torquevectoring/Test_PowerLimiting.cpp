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
PowerLimiting_Inputs* power_limiting_inputs_test1;
power_limiting_inputs_test1->left_motor_temp_C = 100.0;
power_limiting_inputs_test1->right_motor_temp_C =  100.0;
power_limiting_inputs_test1->available_power_kW = POWER_LIMIT_CAR_kW
power_limiting_inputs_test1->accelerator_pedal_percent = 1.00; 

TEST_F(PowerLimitingTest, power_limiting_inputs_test1)
{
    float expected_power_limit_test1 = POWER_LIMIT_CAR_kW - (fmaxf(power_limiting_inputs_test1->left_motor_temp_C, power_limiting_inputs_test1->right_motor_temp_C)- MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float actual_power_limit_test1 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test1);
    ASSERT_FLOAT_EQ(actual_power_limit_test1, expected_power_limit_test1);
}


// Test2: Limit on BMS available power
PowerLimiting_Inputs* power_limiting_inputs_test2;
power_limiting_inputs_test2->left_motor_temp_C = 10.0;
power_limiting_inputs_test2->right_motor_temp_C =  10.0;
power_limiting_inputs_test2->available_power_kW = 50.0;
power_limiting_inputs_test2->accelerator_pedal_percent = 1.00; 


TEST_F(PowerLimitingTest, power_limiting_inputs_test2)
{
    float expected_power_limit_test2 = 50.0;
    float actual_power_limit_test2 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test2);
    ASSERT_FLOAT_EQ(actual_power_limit_test2, expected_power_limit_test2);
}


// Test3: Limit on pedal percentage
PowerLimiting_Inputs* power_limiting_inputs_test3;
power_limiting_inputs_test3->left_motor_temp_C = 10.0;
power_limiting_inputs_test3->right_motor_temp_C =  10.0;
power_limiting_inputs_test3->available_power_kW = POWER_LIMIT_CAR_kW;
power_limiting_inputs_test3->accelerator_pedal_percent = 0.50; 

TEST_F(PowerLimitingTest, power_limiting_inputs_test3)
{
    float expected_power_limit_test3 = POWER_LIMIT_CAR_kW * power_limiting_inputs_test3->accelerator_pedal_percent;
    float actual_power_limit_test3 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test3);
    ASSERT_FLOAT_EQ(actual_power_limit_test3, expected_power_limit_test3);
}


// Test4 Pedal not pressed
PowerLimiting_Inputs* power_limiting_inputs_test4;
power_limiting_inputs_test4->left_motor_temp_C = 10.0;
power_limiting_inputs_test4->right_motor_temp_C =  10.0;
power_limiting_inputs_test4->available_power_kW = POWER_LIMIT_CAR_kW;
power_limiting_inputs_test4->accelerator_pedal_percent = 0.00; 

TEST_F(PowerLimitingTest, power_limiting_inputs_test4)
{
    float expected_power_limit_test4 = 0.0;
    float actual_power_limit_test4 = App_PowerLimiting_ComputeMaxPower(power_limiting_inputs_test4);
    ASSERT_FLOAT_EQ(actual_power_limit_test4, expected_power_limit_test4);
}