#include <math.h>
#include "Io_VoltageSense.h"

float Io_VoltageSense_GetTractiveSystemVoltage(void)
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
    // TS_Voltage = ----------------------------------
    //                Voltage Ratio x Amplifier Gain

    //TODO: TS_VOLTAGE_TUTORIAL: collect the analog value from the Adc1Channel3 and store it as adc_voltage.
    //                          Reference Io_Adc.h for function prototypes
    float adc_voltage = 0;

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }
    else
    {
        //TODO: TS_VOLTAGE_TUTORIAL: return the ts_voltage by converting the adc_voltage using the function above
        //                          Reference Io_VoltageSense.c for #define values
        return 0;
    }
}
