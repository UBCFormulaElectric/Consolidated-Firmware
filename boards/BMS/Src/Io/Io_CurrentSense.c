#include <stddef.h>
#include <math.h>
#include "Io_CurrentSense.h"

// Offset voltage of output 1. Found to be 2.45V through testing
#define OUTPUT_1_OFFSET (2.45f)
// Sensitivity of output 1: 40mV/A
#define OUTPUT_1_SENSITIVITY (40e-3f)
// Voltage divider between sensor output and adc input.
#define OUTPUT_1_DIV (2.155f / (1.1f + 2.155f))

// Offset voltage of output 2. Found to be 2.45V through testing
#define OUTPUT_2_OFFSET (2.45f)
// Sensitivity of output 2: 6.67mV/A
#define OUTPUT_2_SENSITIVITY (6.67e-3f)
// Voltage divider between sensor output and adc input.
#define OUTPUT_2_DIV (2.155f / (1.1f + 2.155f))

// Gain of the current sense amplifier
#define AIR_LOOP_GAIN (20.0f)
// Value of shunt resistor
#define AIR_LOOP_SHUNT_RES (75.0e-3f)

float Io_CurrentSense_GetHighResolutionMainCurrent(float adc_voltage)
{
    if (adc_voltage < 0.0f)
        return NAN;

    // HSNBV-D06 Output 1 (+/- 50A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1.1k>---+---| ADC Channel |
    // | Output 1 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //             HSNBV-D06 Output 1 Voltage - Offset Voltage
    // Current = ------------------------------------------------
    //                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 1 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 40mV/A

    const float hsnbv_d06_output_1 = adc_voltage * 1.0f / OUTPUT_1_DIV;

    float high_res_main_current = (hsnbv_d06_output_1 - OUTPUT_1_OFFSET) / OUTPUT_1_SENSITIVITY;

    return high_res_main_current;
}

float Io_CurrentSense_GetLowResolutionMainCurrent(float adc_voltage)
{
    if (adc_voltage < 0.0f)
        return NAN;

    // HSNBV-D06 Output 2 (+/- 300A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1.1k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //             HSNBV-D06 Output 2 Voltage - Offset Voltage
    // Current = ------------------------------------------------
    //                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 2 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 6.67mV/A

    const float hsnbv_d06_output_2 = adc_voltage * 1 / OUTPUT_2_DIV;

    float low_res_main_current = (hsnbv_d06_output_2 - OUTPUT_2_OFFSET) / OUTPUT_2_SENSITIVITY;

    return low_res_main_current;
}

float Io_CurrentSense_GetAirLoopCurrent(float adc_voltage)
{
    if (adc_voltage < 0.0f)
        return NAN;

    //   +-------<0.075>-------+
    //   |                     |
    //   |  +---------------+  |
    //   +--| Current Sense |--+
    //      |   Amplifier   |------
    //      +---------------+
    //
    // Gain = 20V/V
    //                   ADC Voltage
    // Current = -----------------------------
    //              Sense Resistance * Gain

    float air_loop_current = adc_voltage / (AIR_LOOP_SHUNT_RES * AIR_LOOP_GAIN);

    return air_loop_current;
}
