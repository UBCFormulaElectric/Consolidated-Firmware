#include "io_mechanicalLoad.h"

// the S-Type load cell like the DYLY-103, it actually requires an excitation voltage to function correctly.
// This type of load cell is a strain gauge-based transducer that converts mechanical force into an electrical output
// signal that is proportional to the force.

// assumed excitation voltage
#define EXCITATION_VOLTAGE (10.0f)
#define SENSETIVITY_FACTOR (0.002f) // 2mv/V
#define CONVERSION(voltage) ((voltage) / (SENSETIVITY_FACTOR * EXCITATION_VOLTAGE))

float io_voltageToMechanicalLoad(float voltage)
{
    return CONVERSION(voltage);
}
