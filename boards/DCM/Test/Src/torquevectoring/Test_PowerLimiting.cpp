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

TEST(PowerLimitingTest, motor_temps_limit_power)
{
    struct PowerLimiting_Inputs* test1_inputs = (struct PowerLimiting_Inputs *)malloc(sizeof(struct PowerLimiting_Inputs));
    *test1_inputs = {100.0, 100.0, POWER_LIMIT_CAR_kW, 1.00};

    float expected_power_limit_test1 = POWER_LIMIT_CAR_kW - (fmaxf(test1_inputs->left_motor_temp_C, test1_inputs->right_motor_temp_C)- MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float actual_power_limit_test1 = App_PowerLimiting_ComputeMaxPower(test1_inputs);
    // Deallocate memory
    ASSERT_FLOAT_EQ(actual_power_limit_test1, expected_power_limit_test1);
    free(test1_inputs);
}


// Test2: Limit on BMS available power

TEST(PowerLimitingTest, bms_limits_power)
{
    // struct PowerLimiting_Inputs* test1_inputs = (struct PowerLimiting_Inputs *)malloc(sizeof(struct PowerLimiting_Inputs));
   struct PowerLimiting_Inputs* test2_inputs =  (struct PowerLimiting_Inputs *)malloc(sizeof(struct PowerLimiting_Inputs));
   *test2_inputs = {10.0, 10.0, 50.0, 1.00};
   float expected_power_limit_test2 = 50.0;
   float actual_power_limit_test2 = App_PowerLimiting_ComputeMaxPower(test2_inputs);
   ASSERT_FLOAT_EQ(actual_power_limit_test2, expected_power_limit_test2);
   free(test2_inputs);
}


// Test3: Limit on pedal percentage

TEST(PowerLimitingTest, pedal_limits_power)
{
    struct PowerLimiting_Inputs* test3_inputs = (struct PowerLimiting_Inputs *)malloc(sizeof(struct PowerLimiting_Inputs));
   *test3_inputs = {10.0, 10.0, POWER_LIMIT_CAR_kW, 0.50};
   float expected_power_limit_test3 = POWER_LIMIT_CAR_kW * test3_inputs->accelerator_pedal_percent;
   float actual_power_limit_test3 = App_PowerLimiting_ComputeMaxPower(test3_inputs);
   ASSERT_FLOAT_EQ(actual_power_limit_test3, expected_power_limit_test3);
   free(test3_inputs);
}


// Test4 Pedal not pressed

TEST(PowerLimitingTest, no_power_when_pedal_not_pressed)
{
   struct PowerLimiting_Inputs* test4_inputs = (struct PowerLimiting_Inputs *)malloc(sizeof(struct PowerLimiting_Inputs));
   *test4_inputs = {10.0, 10.0, POWER_LIMIT_CAR_kW, 0.00};
   float expected_power_limit_test4 = 0.0;
   float actual_power_limit_test4 = App_PowerLimiting_ComputeMaxPower(test4_inputs);
   ASSERT_FLOAT_EQ(actual_power_limit_test4, expected_power_limit_test4);
   free(test4_inputs);
}