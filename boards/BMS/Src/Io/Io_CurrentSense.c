#include <stddef.h>
#include "Io_CurrentSense.h"

ExitCode Io_CurrentSense_ConvertToLowResolutionMainCurrent(float adc_voltage, float *low_res_main_current)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (low_res_main_current == NULL)
        return EXIT_CODE_INVALID_ARGS;

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

    const float offset             = 2.5f;
    const float sensitivity        = 40e-3f;
    const float hsnbv_d06_output_1 = adc_voltage * (1e3f + 2.2e3f) / 2.2e3f;

    *low_res_main_current = (hsnbv_d06_output_1 - offset) / sensitivity;

    return EXIT_CODE_OK;
}

ExitCode Io_CurrentSense_ConvertToHighResolutionMainCurrent(float adc_voltage, float *high_res_main_current)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (high_res_main_current == NULL)
        return EXIT_CODE_INVALID_ARGS;

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

    const float offset             = 2.5f;
    const float sensitivity        = 6.67e-3f;
    const float hsnbv_d06_output_2 = adc_voltage * (1e3f + 2.2e3f) / 2.2e3f;

    *high_res_main_current = (hsnbv_d06_output_2 - offset) / sensitivity;

    return EXIT_CODE_OK;
}

ExitCode Io_CurrentSense_ConvertToAirLoopCurrent(float adc_voltage, float *air_loop_current)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (air_loop_current == NULL)
        return EXIT_CODE_INVALID_ARGS;

    //                   ADC Voltage
    // Current = -----------------------------
    //              Sense Resistance * Gain

    const float sense_resistance_ohms = 75e-3f;
    const float gain                  = 20.0f;

    *air_loop_current = adc_voltage / (sense_resistance_ohms * gain);

    return EXIT_CODE_OK;
}
