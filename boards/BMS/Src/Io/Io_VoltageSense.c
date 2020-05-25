#include <stddef.h>
#include "Io_VoltageSense.h"

ExitCode Io_VoltageSense_GetTractiveSystemVoltage(
    float  adc_voltage,
    float *tractive_system_voltage)
{
    if (adc_voltage < 0.0f)
        return EXIT_CODE_OUT_OF_RANGE;

    if (tractive_system_voltage == NULL)
        return EXIT_CODE_INVALID_ARGS;

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
    // TS_Voltage = ----------------------------------
    //                Voltage Ratio x Amplifier Gain

    const float voltage_ratio =
        1.024e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.024e+3f);
    const float amplifier_gain = 8.0f;

    *tractive_system_voltage = adc_voltage / (voltage_ratio * amplifier_gain);

    return EXIT_CODE_OK;
}
