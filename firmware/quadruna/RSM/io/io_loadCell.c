#include "app_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_loadCell.h"

#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

float loadCell_voltageToMechancialLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://www.amazon.ca/Portable-High-Precision-Pressure-Tension-Weighing/dp/B077YHNNCP/ref=sr_1_4?crid=1OS08NST7MBPQ&keywords=psd%2Bload%2Bcell&qid=1698866245&sprefix=psd%2Bload%2Bce%2Caps%2C143&sr=8-4&th=1)
    return 0.0;
}

bool loadCell_OCSC(int adcPin_toCheck)
{
    const float voltage = hw_adc_getVoltage(ADC1_IN10_LC3_OUT);
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad3(void);
{
    return loadCell_voltageToMechancialLoad(hw_adc_getVoltage(ADC1_IN10_LC3_OUT));
}

float io_loadCell_getMechanicalLoad4(void)
{
    return loadCell_voltageToMechancialLoad(hw_adc_getVoltage(ADC1_IN10_LC4_OUT));
}

bool io_loadCell3_OCSC(void)
{
    return io_loadCell_OCSC(ADC1_IN10_LC3_OUT);
}

bool io_loadCell4_OCSC(void)
{
    return io_loadCell_OCSC(ADC1_IN10_LC4_OUT);
}
