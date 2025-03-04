#include "io_loadCell.h"
#include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

const LoadCell sensor3 = { .src = &lc3_out };

bool io_loadCell_OCSC()
{
    const float voltage = hw_adc_getVoltage(sensor3.src);
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad()
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}
