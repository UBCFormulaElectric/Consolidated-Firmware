#include <math.h>
#include "Io_VoltageSense.h"
#include "Io_Adc.h"

float Io_VoltageSense_GetTractiveSystemVoltage()
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

    const float ts_vsense_p = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_10);
    const float ts_vsense_n = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_11);
    const float ts_vsense   = ts_vsense_p - ts_vsense_n;

    if (ts_vsense < 0.0f)
    {
        return NAN;
    }
    else
    {
        return ts_vsense * R_ERROR_COMPENSATION / (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
    }
}
