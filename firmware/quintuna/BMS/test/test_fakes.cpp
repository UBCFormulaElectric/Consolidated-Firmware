extern "C"
{
#include "io_ltc6813.h"
}

static bool state = false;
void        io_ltc6813_writeConfigurationRegisters_setOutput(bool output)
{
    state = output;
}

static bool startCellAdcConv = false;
void        io_ltc6813_startCellsAdcConversion_setOutput(bool output)
{
    startCellAdcConv = output;
}

extern "C" void io_ltc6813_readVoltages(
    float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
}

static bool startThermAdcConv = false;
void        io_ltc6813_startThermistorsAdcConversion_setOutput(bool output)
{
    startThermAdcConv = output;
}

extern "C" void io_ltc6813_readTemperatures(
    float  cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    float *vref,
    bool   success[NUM_SEGMENTS][AUX_REG_GROUPS])
{
}
