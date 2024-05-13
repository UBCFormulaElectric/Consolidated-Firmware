#include "io_mechanicalLoad.h"

#define LOADCELL_SLOPE 1
#define Y_INTERCEPT 0

float io_voltageToMechanicalLoad(float voltage)
{
    // TODO: find transfer function (will need to be experimentally determined when the load cells come in)
    // Assumed to be a linear function
    return LOADCELL_SLOPE * voltage + Y_INTERCEPT;
}
