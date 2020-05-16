#include <stddef.h>
#include "App_CurrentSense.h"

ExitCode App_CurrentSense_ConvertToMainCurrent1(
    float  adc_voltage,
    float *main_current_1)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (main_current_1 == NULL)
        return EXIT_CODE_INVALID_ARGS;

    // HSNBV-D06 Current Sensor:
    //
    // Output 1 Range = +/- 50A, Sensitivity = 40mV/A, Offset = 2.5V
    //
    //              ADC Voltage - Offset
    // Current = -------------------------
    //                  Sensitivity

    const float offset      = 2.5f;
    const float sensitivity = 40e-3f;

    *main_current_1 = (adc_voltage - offset) / sensitivity;

    return EXIT_CODE_OK;
}

ExitCode App_CurrentSense_ConvertToMainCurrent2(
    float  adc_voltage,
    float *main_current_2)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (main_current_2 == NULL)
        return EXIT_CODE_INVALID_ARGS;

    // HSNBV-D06 Current Sensor:
    //
    // Output 2 Range = +/- 300A, Sensitivity = 6.67mV/A, Offset = 2.5V
    //
    //              ADC Voltage - Offset
    // Current = -------------------------
    //                  Sensitivity

    const float offset      = 2.5f;
    const float sensitivity = 6.67e-3f;

    *main_current_2 = (adc_voltage - offset) / sensitivity;

    return EXIT_CODE_OK;
}

ExitCode App_CurrentSense_ConvertToAirLoopCurrent(
    float  adc_voltage,
    float *air_loop_current)
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
