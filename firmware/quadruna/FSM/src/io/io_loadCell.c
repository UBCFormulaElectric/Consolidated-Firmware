#include "io_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "hw_adc.h"
#include "io_loadCell.h"

// TODO: define these constants
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

float voltageToMechanicalLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://www.amazon.ca/Portable-High-Precision-Pressure-Tension-Weighing/dp/B077YHNNCP/
    return voltage;
}

bool loadCellOCSC(float voltage)
{
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad1(void)
{
    return voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN13_LOAD_CELL_1));
}

float io_loadCell_getMechanicalLoad2(void)
{
    return voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN1_LOAD_CELL_2));
}

bool io_loadCell_sensor1OCSC(void)
{
    return loadCellOCSC(hw_adc_getVoltage(ADC1_IN13_LOAD_CELL_1));
}

bool io_loadCell_sensor2OCSC(void)
{
    return loadCellOCSC(hw_adc_getVoltage(ADC1_IN1_LOAD_CELL_2));
}
