#include "app/segments.h"
#include "app/segments/segments_internal.h"

#include "app/utils.h"
#include "io/ltc6813.h"
#include "io/time.h"

#include <stdint.h>
#include <float.h>

// TODO: Take a closer look at these guys
#define CONVERSION_TIME_MS (10)
#define OWC_CONVERSION_TIME_MS (201)

// Keeping separate buffers for every command is pretty wasteful but we have lots of RAM so oh well

uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
ExitCode cell_voltage_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];

uint16_t aux_regs[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
ExitCode aux_regs_success[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
float    segment_vref[NUM_SEGMENTS];
float    cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
ExitCode cell_temps_success[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

StatusRegGroups status_regs[NUM_SEGMENTS];
ExitCode        status_regs_success[NUM_SEGMENTS];
uint16_t        adc_accuracy_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
ExitCode        adc_accuracy_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

uint16_t voltage_self_test_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
ExitCode voltage_self_test_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

uint16_t aux_self_test_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
ExitCode aux_self_test_reg_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];

StatusRegGroups status_self_test_regs[NUM_SEGMENTS];
ExitCode        status_self_test_success[NUM_SEGMENTS];

uint16_t owc_pucv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
ExitCode owc_pucv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
uint16_t owc_pdcv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
ExitCode owc_pdcv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
bool     owc_results[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];
bool     owc_results_success[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];

ExitCode app_segments_runVoltageConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_startCellsAdcConversion());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(cell_voltage_regs, cell_voltage_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAuxConversion(void)
{
    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        app_segments_setThermistorMuxConfig(mux);
        app_segments_writeConfig();

        RETURN_IF_ERR(io_ltc6813_startThermistorsAdcConversion());
        io_time_delay(CONVERSION_TIME_MS);
        io_ltc6813_readAuxRegisters(aux_regs[mux], aux_regs_success[mux]);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_runStatusConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_diagnoseMUX());
    io_time_delay(1); // Empirically you need this small delay after sending DIAGN
    RETURN_IF_ERR(io_ltc6813_startInternalADCConversions());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_getStatus(status_regs, status_regs_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAdcAccuracyTest(void)
{
    RETURN_IF_ERR(io_ltc6813_overlapADCTest());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(adc_accuracy_regs, adc_accuracy_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runVoltageSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestVoltages());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(voltage_self_test_regs, voltage_self_test_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAuxSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestAux());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readAuxRegisters(aux_self_test_regs, aux_self_test_reg_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runStatusSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestStat());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_getStatus(status_self_test_regs, status_self_test_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runOpenWireCheck(void)
{
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_UP));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_UP));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(owc_pucv_regs, owc_pucv_success);

    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_DOWN));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_DOWN));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(owc_pdcv_regs, owc_pdcv_success);

    return EXIT_CODE_OK;
}
