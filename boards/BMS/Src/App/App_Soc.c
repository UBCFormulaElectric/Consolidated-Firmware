#include <stdint.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "App_Soc.h"
#include "App_SharedProcessing.h"
#include "App_SharedConstants.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define EFFECTIVE_INTERNAL_R_OHMS (INTERNAL_R_PER_CELL_OHMS / (float)NUM_PARALLEL_CELLS)

struct CellSoc
{
    // Energy loss at time t
    float curr_energy_j;

    // Energy loss at time t-1
    float prev_losses_j;

    // Accumulated energy losses (J)
    float losses_integral_j;
};

struct Soc
{
    struct CellSoc cell_energy[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
};

static float App_Soc_GetSocFromVoltageLookup(uint8_t segment, uint8_t cell, float curr_cell_v);
static void App_Soc_UpdateCellEnergy(struct Soc* soc, uint8_t segment, uint8_t cell, float current, float cell_voltage, float time_step_s);

static float App_Soc_GetSocFromVoltageLookup(uint8_t segment, uint8_t cell, float curr_cell_v)
{
    uint8_t curr_soc_index;

    // Find the index for the curr cell soc to look up how much energy is available in the cell
    for (curr_soc_index = 0U; curr_cell_v < v_to_index[curr_soc_index]; curr_soc_index++)
        ;
    if (curr_soc_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        curr_soc_index--;
    }

    return index_to_energy[curr_soc_index];;
}

static void App_Soc_UpdateCellEnergy(struct Soc* soc, uint8_t segment, uint8_t cell, float current, float cell_voltage, float time_step_s)
{
    // Current losses = I*(V curr cell) + (Internal R) * I^2
    float curr_losses_w = current * cell_voltage + current * current * EFFECTIVE_INTERNAL_R_OHMS;

    float *integrated_energy_losses_j = &soc->cell_energy[segment][cell].losses_integral_j;
    float *prev_losses_w              = &soc->cell_energy[segment][cell].prev_losses_j;
    float *energy                     = &soc->cell_energy[segment][cell].curr_energy_j;

    // Update soc losses integral
    App_SharedProcessing_TrapezoidalRule(integrated_energy_losses_j, prev_losses_w, curr_losses_w, time_step_s);

    // Update the SoC
    *energy = *energy - *integrated_energy_losses_j;
}

struct Soc* App_Soc_Create(struct Accumulator* accumulator)
{
    struct Soc* soc = malloc(sizeof(struct Soc));
    
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            const float cell_v = App_Accumulator_GetCellVoltage(accumulator, segment, cell);
            soc->cell_energy[segment][cell].curr_energy_j = App_Soc_GetSocFromVoltageLookup(segment, cell, cell_v);
            soc->cell_energy[segment][cell].prev_losses_j = 0.0f;
            soc->cell_energy[segment][cell].losses_integral_j = 0.0f;
        }
    }

    return soc;
}

void App_Soc_Destroy(struct Soc* soc)
{
    free(soc);
}

ExitCode App_Soc_Vote(float max_abs_difference, float soc_1, float soc_2, float soc_3, float *result)
{
    if (max_abs_difference < 0.0f || max_abs_difference > 100.0f || soc_1 < 0.0f || soc_1 > 100.0f || soc_2 < 0.0f ||
        soc_2 > 100.0f || soc_3 < 0.0f || soc_3 > 100.0f)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    float diff = 0.0f;

    diff = fabsf(soc_1 - soc_2);
    if (diff <= max_abs_difference)
    {
        *result = (soc_1 + soc_2) / 2.0f;
        return EXIT_CODE_OK;
    }

    diff = fabsf(soc_2 - soc_3);
    if (diff <= max_abs_difference)
    {
        *result = (soc_2 + soc_3) / 2.0f;
        return EXIT_CODE_OK;
    }

    diff = fabsf(soc_1 - soc_3);
    if (diff <= max_abs_difference)
    {
        *result = (soc_1 + soc_3) / 2.0f;
        return EXIT_CODE_OK;
    }

    *result = NAN;
    return EXIT_CODE_OK;
}

void App_Soc_UpdatePackEnergy(struct Soc* soc, struct Accumulator* accumulator, struct TractiveSystem* ts, float time_step_s)
{
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            const float ts_current = App_TractiveSystem_GetCurrent(ts);
            const float cell_voltage = App_Accumulator_GetCellVoltage(accumulator, segment, cell);
            App_Soc_UpdateCellEnergy(soc, segment, cell, ts_current, cell_voltage, time_step_s);
        }
    }
}

float App_Soc_GetMinCellEnergyInPercent(struct Soc* soc, uint8_t *segment, uint8_t *cell)
{
    float min_energy = FLT_MAX;
    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; curr_cell++)
        {
            const float curr_energy = soc->cell_energy[curr_segment][curr_cell].curr_energy_j;
            if (curr_energy < min_energy)
            {
                min_energy = curr_energy;
                *segment   = curr_segment;
                *cell      = curr_cell;
            }
        }
    }

    // Return as a percentage between [0,100]
    return 100.0f * min_energy / index_to_energy[0];
}

float App_Soc_GetMaxCellEnergyInPercent(struct Soc* soc, uint8_t *segment, uint8_t *cell)
{
    float max_energy = 0;
    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; curr_cell++)
        {
            const float curr_energy = soc->cell_energy[curr_segment][curr_cell].curr_energy_j;
            if (curr_energy > max_energy)
            {
                max_energy = curr_energy;
                *segment   = curr_segment;
                *cell      = curr_cell;
            }
        }
    }

    // Return as a percentage between [0,100]
    return 100.0f * max_energy / index_to_energy[0];
}
