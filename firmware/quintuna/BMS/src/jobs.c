#include "jobs.h"

#include "app_canTx.h"
#include "io_ltc6813.h"

void jobs_init(void) {}

void jobs_run1Hz_tick(void) {}
void jobs_run100Hz_tick(void) {}

void jobs_runLtc(void)
{
    static float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

    static bool voltage_read_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
    static bool temp_read_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]        = { false };

    static bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    io_ltc6813_writeConfigurationRegisters(balance_config); // no balancing

    io_ltc6813_startCellsAdcConversion(ADCSpeed_3kHz);
    io_ltc6813_readVoltages(cell_voltages, voltage_read_success);

    float vref = 0.0f;
    io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz);
    io_ltc6813_readTemperatures(cell_temps, &vref, temp_read_success);
}