#include <math.h>
#include "Test_Bms.h"

extern "C"
{
#include "App_VoltageSense.h"
}

TEST(VoltageSenseTest, tractive_system_voltage_calculation)
{
    // Negative ADC voltage
    float adc_voltage =
        std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    float tractive_system_voltage = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE, App_VoltageSense_GetTractiveSystemVoltage(
                                    adc_voltage, &tractive_system_voltage));

    // Zero tractive system voltage (0V)
    adc_voltage = 0.0f;
    ASSERT_EQ(
        EXIT_CODE_OK, App_VoltageSense_GetTractiveSystemVoltage(
                          adc_voltage, &tractive_system_voltage));
    ASSERT_EQ(0.0f, tractive_system_voltage);

    // Nominal tractive system voltage (400V)
    adc_voltage =
        400.0f * 8.0f *
        (1.024e+3f / (1.024e+3f + 499e+3f + 499e+3f + 499e+3f + 499e+3f));
    ASSERT_EQ(
        EXIT_CODE_OK, App_VoltageSense_GetTractiveSystemVoltage(
                          adc_voltage, &tractive_system_voltage));
    ASSERT_EQ(400.0f, tractive_system_voltage);

    // Null pointer
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_VoltageSense_GetTractiveSystemVoltage(adc_voltage, NULL));
}
