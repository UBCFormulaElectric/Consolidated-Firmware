#include "io_loadCell.h"

#include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

float loadCell_voltageToMechancialLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}

bool loadCell_OCSC(const AdcChannel *c)
{
    const float voltage = hw_adc_getVoltage(c);
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad3(void)
{
    return loadCell_voltageToMechancialLoad(hw_adc_getVoltage(&lc3));
}

float io_loadCell_getMechanicalLoad4(void)
{
    return loadCell_voltageToMechancialLoad(hw_adc_getVoltage(&lc4));
}

bool io_loadCell_sensor3OCSC(void)
{
    return loadCell_OCSC(&lc3);
}

bool io_loadCell_sensor4OCSC(void)
{
    return loadCell_OCSC(&lc4);
}
