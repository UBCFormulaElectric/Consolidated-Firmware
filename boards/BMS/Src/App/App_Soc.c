#include <stdint.h>
#include <float.h>
#include <math.h>
#include "App_Soc.h"
#include "App_SharedConstants.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define EFFECTIVE_INTERNAL_R_OHMS (INTERNAL_R_PER_CELL_OHMS / (float)NUM_PARALLEL_CELLS)
#define SERIES_ELEMENT_FULL_CHARGE_C (5.9f * 3600.0f * 3.0f)

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

float App_Soc_GetCell_Soc(CellSocStats *cell_stats)
{
    return cell_stats->charge_c / SERIES_ELEMENT_FULL_CHARGE_C * 100.0f;
}

// TODO: Eventually this should take an input to read previous soc stats off of the eeprom instead of resetting SOC
// every time.
struct SocStats *App_SocStats_Create(void)
{
    struct SocStats *soc_stats = malloc(sizeof(struct SocStats));
    assert(soc_stats != NULL);

    // RESETTING ON INSTANTIATION, UPDATE THIS WITH EEPROM VALUES IF AVAILIBLE
    App_SocStats_ResetSoc(soc_stats);

    return soc_stats;
}

void App_SocStats_Destroy(struct SocStats *soc_stats)
{
    free(soc_stats);
}

void App_SocStats_ResetCellSoc(CellSocStats *cell_stats)
{
    cell_stats->charge_c          = SERIES_ELEMENT_FULL_CHARGE_C;
    cell_stats->prev_current_A    = 0.0f;
    cell_stats->charge_integral_c = 0.0f;
}

void App_SocStats_ResetSoc(struct SocStats *soc_stats)
{
    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; curr_cell++)
        {
            App_SocStats_ResetCellSoc(&soc_stats->cell_stats[curr_segment][curr_cell]);
        }
    }
}

void App_SocStats_UpdateSocStats(struct SocStats *soc_stats, float current, float time_step_s)
{
    // NOTE current sign is relative to current into the battery

    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; curr_cell++)
        {
            // change in charge = ∫I*dt

            float *integrated_charge_value = &soc_stats->cell_stats[curr_segment][curr_cell].charge_integral_c;
            float *prev_current            = &soc_stats->cell_stats[curr_segment][curr_cell].prev_current_A;

            // Trapezoidal Rule adds integral of current time-step to previous integral value.
            App_SharedProcessing_TrapezoidalRule(integrated_charge_value, prev_current, current, time_step_s);

            soc_stats->cell_stats[curr_segment][curr_cell].charge_c =
                SERIES_ELEMENT_FULL_CHARGE_C - *integrated_charge_value;
        }
    }
}
