extern "C"
{
#include "io_ltc6813.h"
}

static bool state = false;
void        io_ltc6813_writeConfigurationRegisters_setOutput(const bool output)
{
    state = output;
}

extern "C" bool io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    return state;
}

static bool startCellAdcConv = false;
void        io_ltc6813_startCellsAdcConversion_setOutput(const bool output)
{
    startCellAdcConv = output;
}
extern "C" bool io_ltc6813_startCellsAdcConversion(const ADCSpeed speed)
{
    return startCellAdcConv;
}

extern "C" void io_ltc6813_readVoltages(
    float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
}

static bool startThermAdcConv = false;
void        io_ltc6813_startThermistorsAdcConversion_setOutput(const bool output)
{
    startThermAdcConv = output;
}
extern "C" bool io_ltc6813_startThermistorsAdcConversion(const ADCSpeed speed)
{
    return startThermAdcConv;
}

extern "C" void io_ltc6813_readTemperatures(
    float  cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    float *vref,
    bool   success[NUM_SEGMENTS][AUX_REGISTER_GROUPS])
{
}
