#include "io_loadCell.h"
// #include "hw_adc.h"
// #include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

const LoadCell sensor3 = { .src = &lc3_out };
const LoadCell sensor4 = { .src = &lc4_out };

bool loadCell_OCSC(LoadCell *loadCell)
{
    const float voltage = hw_adc_getVoltage(&loadCell->src);
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float loadCell_getMechanicalLoad(LoadCell *loadCell)
{
    return voltageToMechanical(hw_adc_getVoltage(&loadCell->src));
}

float loadCell_voltageToMechancialLoad(LoadCell *loadCell, float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}
