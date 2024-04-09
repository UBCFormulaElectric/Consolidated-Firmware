#include "io_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "hw_adc.h"
#include "io_loadCell.h"
#include "io_mechanicalLoad.h"

// TODO: define these constants
#define LOADCELL_MIN_VOLTAGE (0.0f)
#define LOADCELL_MAX_VOLTAGE (1000.0f)

bool io_loadCell_OCSC(float voltage)
{
    return !(LOADCELL_MIN_VOLTAGE <= voltage && voltage <= LOADCELL_MAX_VOLTAGE);
}

float io_loadCell_getMechanicalLoad1(void)
{
    return io_voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN13_LOAD_CELL_1));
}

float io_loadCell_getMechanicalLoad2(void)
{
    return io_voltageToMechanicalLoad(hw_adc_getVoltage(ADC1_IN1_LOAD_CELL_2));
}

bool io_loadCell_sensor1OCSC(void)
{
    return io_loadCell_OCSC(hw_adc_getVoltage(ADC1_IN13_LOAD_CELL_1));
}

bool io_loadCell_sensor2OCSC(void)
{
    return io_loadCell_OCSC(hw_adc_getVoltage(ADC1_IN1_LOAD_CELL_2));
}
