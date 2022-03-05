#include <math.h>
#include "Io_VoltageSense.h"
#include "Io_Adc.h"

float Io_VoltageSense_GetTractiveSystemVoltage()
{
    // The tractive system voltage is divided down by several resistors, then
    // fed through an amplifier. Output of amplifier is differential so
    // TS_VSENSE_P will be half the desired output with a common mode voltage
    // of 1.288.
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
    // TS Voltage x Voltage Ratio x Amplifier Gain x Error Correction Constant =
    // ADC voltage, where
    //
    //                              1.24k
    // Voltage Ratio = -------------------------------------
    //                   499k + 499k + 499k + 499k + 1.24k
    //
    // Amplifier Gain = 8
    //
    // Error Correction: value is found through testing. Error can be caused by
    // resistor tolerances and other factors
    //
    // We can rewrite the earlier equation as:
    //
    //                         ADC voltage
    // TS_Voltage = ----------------------------------  * Error Correction
    // Constant
    //                Voltage Ratio x Amplifier Gain

    float adc_voltage = Io_Adc_GetAdc1Channel3Voltage();

    const float voltage_ratio =
        1.24e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.24e+3f);
    const float amplifier_gain        = 8.0f;
    const float real_error_percentage = 5.0f;

    return (adc_voltage) / (voltage_ratio * amplifier_gain) *
           (1 + real_error_percentage / 100);
}
