#include "Io_VoltageSense.h"

#include <math.h>

#include "Io_Adc.h"

float Io_VoltageSense_GetTractiveSystemVoltage() {
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

  const float adc_voltage = 0;  // TODO: Single-ended TS voltage

  if (adc_voltage < 0.0f) {
    return NAN;
  } else {
    return adc_voltage * R_ERROR_COMPENSATION /
           (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
  }
}
