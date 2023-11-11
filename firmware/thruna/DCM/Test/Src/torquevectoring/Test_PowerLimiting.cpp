/**
 *
 * Test_PowerLimiting.cpp
 *
 * Areas to test:
 * 1. Motor temps limit power
 * 2. BMS limits power
 * 3. Pedal position limits power
 * 4. Pedal position allows max power
 * 5. BMS allows no power
 * 6. Motor temps allow no power
 * 7. Motor temps past cutoff allow no power
 * 8. Motor temps at cutoff allow 50kW
 * 9. Pedal not pressed allows no power
 *
 */

#include "Test_Dcm.h"

extern "C"
{
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "App_SharedDcmConstants.h"
#include "math.h"
}

/**
 * Define SetUp and TearDown to be called before every TEST_F call
 */
class PowerLimitingTest : public testing::Test
{
  protected:
    // TODO: Find out if this is necessary if no IO functions are called.
    void SetUp() override { return; }
    void TearDown() override { return; }
};

/**
 * Test 1: Test that the power limiting function limits power when motor temps are high
 */
TEST(PowerLimitingTest, motor_temps_limit_power)
{
    PowerLimiting_Inputs test1_inputs = { 100.0, 100.0, POWER_LIMIT_CAR_kW, 1.00 };
    float                expected_power_limit_test1 =
        POWER_LIMIT_CAR_kW -
        (fmaxf(test1_inputs.left_motor_temp_C, test1_inputs.right_motor_temp_C) - MOTOR_TEMP_CUTOFF_c) *
            MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float actual_power_limit_test1 = App_PowerLimiting_ComputeMaxPower(&test1_inputs);
    // Deallocate memory
    ASSERT_FLOAT_EQ(actual_power_limit_test1, expected_power_limit_test1);
}

/**
 * Test 2: Test that the power limiting function limits power when BMS available power is less than 80kW
 */
TEST(PowerLimitingTest, bms_limits_power)
{
    PowerLimiting_Inputs test2_inputs               = { 10.0, 10.0, 50.0, 1.00 };
    float                expected_power_limit_test2 = 50.0;
    float                actual_power_limit_test2   = App_PowerLimiting_ComputeMaxPower(&test2_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test2, expected_power_limit_test2);
}

/**
 * Test 3: Test that the power limiting function limits power when pedal position is less than 50%
 */
TEST(PowerLimitingTest, pedal_limits_power)
{
    PowerLimiting_Inputs test3_inputs               = { 10.0, 10.0, POWER_LIMIT_CAR_kW, 0.50 };
    float                expected_power_limit_test3 = POWER_LIMIT_CAR_kW * test3_inputs.accelerator_pedal_percent;
    float                actual_power_limit_test3   = App_PowerLimiting_ComputeMaxPower(&test3_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test3, expected_power_limit_test3);
}

/**
 * Test 4: Test that the power limiting function reaches 80kW when pedal is fully pressed
 */
TEST(PowerLimitingTest, max_pedal_position)
{
    PowerLimiting_Inputs test4_inputs               = { 10.0, 10.0, POWER_LIMIT_CAR_kW, 1.00 };
    float                expected_power_limit_test4 = POWER_LIMIT_CAR_kW;
    float                actual_power_limit_test4   = App_PowerLimiting_ComputeMaxPower(&test4_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test4, expected_power_limit_test4);
}

/**
 * Test 5: Test that the power limiting function allows no power when BMS available power is 0
 */
TEST(PowerLimitingTest, bms_allows_no_power)
{
    PowerLimiting_Inputs test5_inputs               = { 10.0, 10.0, 0.0, 1.00 };
    float                expected_power_limit_test5 = 0.0;
    float                actual_power_limit_test5   = App_PowerLimiting_ComputeMaxPower(&test5_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test5, expected_power_limit_test5);
}

/**
 * Test 6: Test that the power limiting function allows no power when motor temps are high
 */
TEST(PowerLimitingTest, motor_temps_allow_no_power)
{
    PowerLimiting_Inputs test6_inputs               = { 120.0, 120.0, POWER_LIMIT_CAR_kW, 1.00 };
    float                expected_power_limit_test6 = 0.0;
    float                actual_power_limit_test6   = App_PowerLimiting_ComputeMaxPower(&test6_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test6, expected_power_limit_test6);
}

/**
 * Test 7: Test that the power limiting function allows no power when motor temps are past the cutoff
 */
TEST(PowerLimitingTest, motor_temps_past_no_power_limit)
{
    PowerLimiting_Inputs test7_inputs               = { 130.0, 130.0, POWER_LIMIT_CAR_kW, 1.00 };
    float                expected_power_limit_test7 = 0.0;
    float                actual_power_limit_test7   = App_PowerLimiting_ComputeMaxPower(&test7_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test7, expected_power_limit_test7);
}

/**
 * Test 8: Test that the power limiting function allows 50kW when motor temps are at the cutoff
 */
TEST(PowerLimitingTest, motor_temps_allow_50kW)
{
    PowerLimiting_Inputs test8_inputs               = { 101.25, 101.25, POWER_LIMIT_CAR_kW, 1.00 };
    float                expected_power_limit_test8 = 50.0;
    float                actual_power_limit_test8   = App_PowerLimiting_ComputeMaxPower(&test8_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test8, expected_power_limit_test8);
}

/**
 * Test 9: Test that the power limiting function allows no power when pedal is not pressed
 */
TEST(PowerLimitingTest, no_power_when_pedal_not_pressed)
{
    PowerLimiting_Inputs test9_inputs               = { 10.0, 10.0, POWER_LIMIT_CAR_kW, 0.00 };
    float                expected_power_limit_test9 = 0.0;
    float                actual_power_limit_test9   = App_PowerLimiting_ComputeMaxPower(&test9_inputs);
    ASSERT_FLOAT_EQ(actual_power_limit_test9, expected_power_limit_test9);
}

/**
 * Test 10: Exhaustive pedal position test
 */
TEST(PowerLimitingTest, exhaustive_pedal_power_test)
{
    float expected_power_limit_test_10;
    float actual_power_limit_test_10;
    for (float pedal_percent = 0; pedal_percent <= 1.0; pedal_percent += 0.01)
    {
        PowerLimiting_Inputs test_10_inputs = { 10.0, 10.0, POWER_LIMIT_CAR_kW, pedal_percent };
        expected_power_limit_test_10        = POWER_LIMIT_CAR_kW * pedal_percent;
        actual_power_limit_test_10          = App_PowerLimiting_ComputeMaxPower(&test_10_inputs);
        ASSERT_FLOAT_EQ(actual_power_limit_test_10, expected_power_limit_test_10);
    }
}