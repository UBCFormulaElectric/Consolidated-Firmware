#include "app_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_loadCell.h"
#include "hw_adc.h"
#include "io_mechanicalLoad.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

bool loadCell_OCSC(int adcPin_toCheck)
{
    const float voltage = hw_adc_getVoltage(ADC1_IN10_LC3_OUT);
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad3(void)
{
    return io_voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN10_LC3_OUT));
}

float io_loadCell_getMechanicalLoad4(void)
{
    return io_voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN0_LC4_OUT));
}

bool io_loadCell3_OCSC(void)
{
    return loadCell_OCSC(ADC1_IN10_LC3_OUT);
}

bool io_loadCell4_OCSC(void)
{
    return loadCell_OCSC(ADC1_IN0_LC4_OUT);
}
