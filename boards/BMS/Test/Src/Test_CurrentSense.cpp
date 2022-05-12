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

TEST_F(CurrentSenseTest, low_res_main_current_calculation)
{
    // Negative ADC voltage
    float adc_voltage          = std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float low_res_main_current = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE, Io_CurrentSense_ConvertToLowResolutionMainCurrent(adc_voltage, &low_res_main_current));

    // Zero current
    adc_voltage = HSNBV_D06_OFFSET_VOLTAGE / voltage_ratio;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToLowResolutionMainCurrent(adc_voltage, &low_res_main_current));
    ASSERT_EQ(0.0f, low_res_main_current);

    // Maximum current for HSNBV-D06 output 1 (+50A)
    adc_voltage = 3.09375f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToLowResolutionMainCurrent(adc_voltage, &low_res_main_current));
    ASSERT_EQ(50.0f, low_res_main_current);

    // Minimum current for HSNBV-D06 output 1 (-50A)
    adc_voltage = 0.34375f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToLowResolutionMainCurrent(adc_voltage, &low_res_main_current));
    ASSERT_EQ(-50.0f, low_res_main_current);

    // Null pointer
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, Io_CurrentSense_ConvertToLowResolutionMainCurrent(adc_voltage, NULL));
}

TEST_F(CurrentSenseTest, high_res_main_current_calculation)
{
    // Negative ADC voltage
    float adc_voltage           = std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float high_res_main_current = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        Io_CurrentSense_ConvertToHighResolutionMainCurrent(adc_voltage, &high_res_main_current));

    // Zero current
    adc_voltage = HSNBV_D06_OFFSET_VOLTAGE / voltage_ratio;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToHighResolutionMainCurrent(adc_voltage, &high_res_main_current));
    ASSERT_EQ(0.0f, high_res_main_current);

    // Maximum current for HSNBV-D06 output 2 (+300A)
    adc_voltage = 3.0944375f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToHighResolutionMainCurrent(adc_voltage, &high_res_main_current));
    ASSERT_EQ(300.0f, high_res_main_current);

    // Minimum current for HSNBV-D06 output 2 (-300A)
    adc_voltage = 0.3430625f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToHighResolutionMainCurrent(adc_voltage, &high_res_main_current));
    ASSERT_EQ(-300.0f, high_res_main_current);

    // Null pointer
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, Io_CurrentSense_ConvertToHighResolutionMainCurrent(adc_voltage, NULL));
}

TEST_F(CurrentSenseTest, air_loop_current_calculation)
{
    // Negative ADC voltage
    float adc_voltage      = std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float air_loop_current = 0.0f;
    ASSERT_EQ(EXIT_CODE_OUT_OF_RANGE, Io_CurrentSense_ConvertToAirLoopCurrent(adc_voltage, &air_loop_current));

    // Zero current
    adc_voltage = 0.0f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToAirLoopCurrent(adc_voltage, &air_loop_current));
    ASSERT_EQ(0.0f, air_loop_current);

    // 1A is roughly the nominal current when the AIR shutdown loop is closed
    adc_voltage = 1.5f;
    ASSERT_EQ(EXIT_CODE_OK, Io_CurrentSense_ConvertToAirLoopCurrent(adc_voltage, &air_loop_current));
    ASSERT_FLOAT_EQ(1.0f, air_loop_current);

    // Null pointer
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, Io_CurrentSense_ConvertToAirLoopCurrent(adc_voltage, NULL));
}
