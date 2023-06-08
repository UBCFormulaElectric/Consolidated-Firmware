#include <stddef.h>
#include <math.h>
#include "Io_CurrentSense.h"
#include "Io_Adc.h"

// Offset voltage of output 1. Found to be 2.45V through testing
#define OUTPUT_1_OFFSET (2.45f)
// Sensitivity of output 1: 40mV/A
#define OUTPUT_1_SENSITIVITY (1.0f / 40e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_1_DIV ((1.0f + 2.2f) / (2.2f))

// Offset voltage of output 2. Found to be 2.45V through testing
#define OUTPUT_2_OFFSET (2.45f)
// Sensitivity of output 2: 6.67mV/A
#define OUTPUT_2_SENSITIVITY (1.0f / 6.67e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_2_DIV ((1.0f + 2.2f) / (2.2f))

// Gain of the current sense amplifier
#define AIR_LOOP_GAIN (1.0f / 20.0f)
// Value of shunt resistor
#define AIR_LOOP_SHUNT_RES (1.0f / 75.0e-3f)

float Io_CurrentSense_GetHighResolutionMainCurrent()
{
    const float adc_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_8);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

    // HSNBV-D06 Output 1 (+/- 50A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1k>---+---| ADC Channel |
    // | Output 1 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                 1
    // Current = (HSNBV-D06 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 1 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 40mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_1 = adc_voltage * OUTPUT_1_DIV;

    // Return the current which corresponds to the output voltage
    return -((hsnbv_d06_output_1 - OUTPUT_1_OFFSET) * OUTPUT_1_SENSITIVITY);
}

float Io_CurrentSense_GetLowResolutionMainCurrent()
{
    const float adc_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_9);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

    // HSNBV-D06 Output 2 (+/- 300A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                 1
    // Current = (HSNBV-D06 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 2 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 6.67mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_2 = adc_voltage * OUTPUT_2_DIV;

    // Return the current which corresponds to the output voltage
    return -((hsnbv_d06_output_2 - OUTPUT_2_OFFSET) * OUTPUT_2_SENSITIVITY);
}

float Io_CurrentSense_GetAirLoopCurrent()
{
    const float adc_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_3);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

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

    // return the current corresponding to the ADC voltage
    return adc_voltage * AIR_LOOP_SHUNT_RES * AIR_LOOP_GAIN;
}
