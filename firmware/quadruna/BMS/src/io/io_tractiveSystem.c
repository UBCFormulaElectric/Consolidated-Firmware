#include "io_tractiveSystem.h"
#include <math.h>
#include "hw_hal.h"

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define R_ERROR_COMPENSATION (1.0f)
// Isolated amplifier gain
#define AMPLIFIER_GAIN (1.0f)
// Voltage divider for the TS+ voltage sensing
#define TS_VOLTAGE_DIV (20e+3f / (6 * 1e+6f + 30e3f + 20e3f))

// Offset voltage of output 1. Found through testing
#define OUTPUT_1_OFFSET (2.593f)
// Sensitivity of output 1: 26.7mV/A
#define OUTPUT_1_SENSITIVITY (1.0f / 26.7e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_1_DIV ((33.0f + 60.4f) / (60.4f))

// Offset voltage of output 2. Found through testing
#define OUTPUT_2_OFFSET (2.558f)
// Sensitivity of output 2: 4.0mV/A
#define OUTPUT_2_SENSITIVITY (1.0f / 4.0e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_2_DIV ((33.0f + 60.4f) / (60.4f))

static const TractiveSystemConfig *config;

void io_tractiveSystem_init(const TractiveSystemConfig *ts_config)
{
    config = ts_config;
}

float io_tractiveSystem_getVoltage()
{
    // The tractive system voltage is divided down by several resistors, then
    // fed through an amplifier.
    //
    // TS Voltage
    //   -+-
    //    |                                     +----------+  +-------------+
    //    +--<499k>--<499k>--<499k>--<499k>--+--| Amplifier|--| ADC Channel |
    //                                       |  +----------+  +-------------+
    //                                       |
    //                                    <1.24k>
    //                                       |
    //                                      ===
    //                                      GND
    //
    // TS Voltage x Voltage Ratio x Amplifier Gain = ADC voltage, where
    //
    //                              1.24k
    // Voltage Ratio = -------------------------------------
    //                   499k + 499k + 499k + 499k + 1.24k
    //
    // Amplifier Gain = 8
    //
    // We can rewrite the earlier equation as:
    //
    //                          ADC voltage
    // TS_Voltage = ---------------------------------- * Measurement Error %
    //                Voltage Ratio x Amplifier Gain

    // TODO: Test differential ADC for voltage measurement
    const float ts_vsense_P = hw_adc_getVoltage(config->ts_vsense_channel_P);
    const float ts_vsense_N = hw_adc_getVoltage(config->ts_vsense_channel_N);
    const float ts_vsense   = ts_vsense_P - ts_vsense_N;

    if (ts_vsense < 0.0f)
    {
        return NAN;
    }
    else
    {
        float real_voltage = ts_vsense * R_ERROR_COMPENSATION / (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
        return real_voltage
    }
}

float io_tractiveSystem_getCurrentHighResolution()
{
    const float adc_voltage = hw_adc_getVoltage(config->ts_isense_high_res_channel);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

    // DHAB S/124 Output 1 (+/- 75A):
    //
    // +------------------+                +-------------+
    // | DHAB S/124       |---<33k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                              <60.4k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                  1
    // Current = (DHAB S/124 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              33k + 60.4k
    // DHAB S/124 Output 2 Voltage = ADC Voltage x --------------
    //                                                 60.4k
    // Offset Voltage = 2.5V (Datasheet)
    //
    // Sensitivity = 26.7mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_1 = adc_voltage * OUTPUT_1_DIV;

    // Return the current which corresponds to the output voltage
    return -((hsnbv_d06_output_1 - OUTPUT_1_OFFSET) * OUTPUT_1_SENSITIVITY);
}

float io_tractiveSystem_getCurrentLowResolution()
{
    const float adc_voltage = hw_adc_getVoltage(config->ts_isense_low_res_channel);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

    // DHAB S/124 Output 2 (+/- 500A):
    //
    // +------------------+                +-------------+
    // | DHAB S/124       |---<33k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                              <60.4k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                  1
    // Current = (DHAB S/124 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              33k + 60.4k
    // DHAB S/124 Output 2 Voltage = ADC Voltage x --------------
    //                                                 60.4k
    // Offset Voltage = 2.5V (Datasheet)
    //
    // Sensitivity = 4.0mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_2 = adc_voltage * OUTPUT_2_DIV;

    // Return the current which corresponds to the output voltage
    return -((hsnbv_d06_output_2 - OUTPUT_2_OFFSET) * OUTPUT_2_SENSITIVITY);
}
