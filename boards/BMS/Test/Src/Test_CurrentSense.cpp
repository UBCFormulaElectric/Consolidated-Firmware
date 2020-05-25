#include <math.h>
#include "Test_Bms.h"

extern "C"
{
#include "Io_CurrentSense.h"
}

class CurrentSenseTest : public testing::Test
{
  protected:
    // The offset voltage is the output voltage when the output current is zero
    static constexpr float HSNBV_D06_OFFSET_VOLTAGE = 2.5f;
    static constexpr float voltage_ratio            = (1e3f + 2.2e3f) / 2.2e3f;
};

TEST_F(CurrentSenseTest, main_current_1_calculation)
{
    // Negative ADC voltage
    float adc_voltage =
        std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float main_current_1 = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_CurrentSense_ConvertToMainCurrent1(adc_voltage, &main_current_1));

    // Zero current
    adc_voltage = HSNBV_D06_OFFSET_VOLTAGE / voltage_ratio;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent1(adc_voltage, &main_current_1));
    ASSERT_EQ(0.0f, main_current_1);

    // Maximum current for HSNBV-D06 output 1 (+50A)
    adc_voltage = 3.09375f;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent1(adc_voltage, &main_current_1));
    ASSERT_EQ(50.0f, main_current_1);

    // Minimum current for HSNBV-D06 output 1 (-50A)
    adc_voltage = 0.34375f;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent1(adc_voltage, &main_current_1));
    ASSERT_EQ(-50.0f, main_current_1);

    // Null pointer
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_CurrentSense_ConvertToMainCurrent1(adc_voltage, NULL));
}

TEST_F(CurrentSenseTest, main_current_2_calculation)
{
    // Negative ADC voltage
    float adc_voltage =
        std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float main_current_2 = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_CurrentSense_ConvertToMainCurrent2(adc_voltage, &main_current_2));

    // Zero current
    adc_voltage = HSNBV_D06_OFFSET_VOLTAGE / voltage_ratio;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent2(adc_voltage, &main_current_2));
    ASSERT_EQ(0.0f, main_current_2);

    // Maximum current for HSNBV-D06 output 2 (+300A)
    adc_voltage = 3.0944375f;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent2(adc_voltage, &main_current_2));
    ASSERT_EQ(300.0f, main_current_2);

    // Minimum current for HSNBV-D06 output 2 (-300A)
    adc_voltage = 0.3430625f;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_CurrentSense_ConvertToMainCurrent2(adc_voltage, &main_current_2));
    ASSERT_EQ(-300.0f, main_current_2);

    // Null pointer
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_CurrentSense_ConvertToMainCurrent2(adc_voltage, NULL));
}

TEST_F(CurrentSenseTest, air_loop_current_calculation)
{
    // Negative ADC voltage
    float adc_voltage =
        std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float air_loop_current = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE, App_CurrentSense_ConvertToAirLoopCurrent(
                                    adc_voltage, &air_loop_current));

    // Zero current
    adc_voltage = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OK, App_CurrentSense_ConvertToAirLoopCurrent(
                          adc_voltage, &air_loop_current));
    ASSERT_EQ(0.0f, air_loop_current);

    // 1A is roughly the nominal current when the AIR shutdown loop is closed
    adc_voltage = 1.5f;
    ASSERT_EQ(
        EXIT_CODE_OK, App_CurrentSense_ConvertToAirLoopCurrent(
                          adc_voltage, &air_loop_current));
    ASSERT_EQ(1.0f, air_loop_current);

    // Null pointer
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_CurrentSense_ConvertToAirLoopCurrent(adc_voltage, NULL));
}
