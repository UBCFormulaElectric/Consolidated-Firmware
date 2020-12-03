#include <math.h>
#include "Test_Bms.h"

extern "C"
{
#include "Io_VoltageSense.h"
}

TEST(VoltageSenseTest, tractive_system_voltage_calculation)
{
    // Negative ADC voltage
    float adc_voltage =
        std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    ASSERT_TRUE(isnanf(Io_VoltageSense_GetTractiveSystemVoltage(adc_voltage)));

    // Zero tractive system voltage (0V)
    adc_voltage = 0.0f;
    ASSERT_EQ(
        adc_voltage, Io_VoltageSense_GetTractiveSystemVoltage(adc_voltage));

    // Nominal tractive system voltage (400V)
    adc_voltage =
        400.0f * 8.0f *
        (1.024e+3f / (1.024e+3f + 499e+3f + 499e+3f + 499e+3f + 499e+3f));
    ASSERT_EQ(400.0f, Io_VoltageSense_GetTractiveSystemVoltage(adc_voltage));
}
