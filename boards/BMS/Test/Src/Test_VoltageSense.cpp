#include "Test_Bms.h"

extern "C"
{
#include "Io_VoltageSense.h"
}

TEST(VoltageSenseTest, tractive_system_voltage_calculation)
{
    // Zero tractive system voltage (0V)
    float adc_voltage = 0.0f;
    ASSERT_EQ(adc_voltage, Io_VoltageSense_GetTractiveSystemVoltage(adc_voltage));

    // Nominal tractive system voltage (400V)
    constexpr float nominal_ts_voltage = 400.0f;
    adc_voltage                        = nominal_ts_voltage * TS_VOLTAGE_DIV * AMPLIFIER_GAIN / R_ERROR_COMPENSATION;
    ASSERT_FLOAT_EQ(nominal_ts_voltage, Io_VoltageSense_GetTractiveSystemVoltage(adc_voltage));
}
