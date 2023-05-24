#include <stdint.h>
#include <float.h>
#include <math.h>
#include "App_Soc.h"
#include "App_SharedConstants.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define STATE_OF_HEALTH (100.0f)
#define SERIES_ELEMENT_FULL_CHARGE_C (5.9f * 3600.0f * 3.0f * STATE_OF_HEALTH)

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
    soc_stats->minSocSegment = 0U;
    soc_stats->minSocSE      = 0U;
    soc_stats->maxSocSegment = 0U;
    soc_stats->maxSocSE      = 0U;
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
    float min_charge = SERIES_ELEMENT_FULL_CHARGE_C;
    float max_charge = 0.0f;

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

            // Update index of series element with the lowest charge
            if (soc_stats->cell_stats[curr_segment][curr_cell].charge_c <= min_charge)
            {
                min_charge               = soc_stats->cell_stats[curr_segment][curr_cell].charge_c;
                soc_stats->minSocSegment = curr_segment;
                soc_stats->minSocSE      = curr_cell;
            }

            // Update index of series element with the highest charge
            if (soc_stats->cell_stats[curr_segment][curr_cell].charge_c >= max_charge)
            {
                max_charge               = soc_stats->cell_stats[curr_segment][curr_cell].charge_c;
                soc_stats->maxSocSegment = curr_segment;
                soc_stats->maxSocSE      = curr_cell;
            }
        }
    }
}

float App_SOC_GetMinVocFromSoc(struct SocStats *soc_stats)
{
    uint8_t min_seg  = soc_stats->minSocSegment;
    uint8_t min_cell = soc_stats->minSocSE;

    float lowest_charge = soc_stats->cell_stats[min_seg][min_cell].charge_c;

    // Find the index for the cell soc to look up the open-circuit voltage
    uint8_t lut_index;

    for (lut_index = 0U; lowest_charge < energy_lookup[lut_index]; lut_index++)
        ;

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        lut_index--;
    }

    return v_to_soc_lookup[lut_index];
}

float App_SOC_GetMaxVocFromSoc(struct SocStats *soc_stats)
{
    uint8_t max_seg = soc_stats->maxSocSegment;
    uint8_t max_SE  = soc_stats->maxSocSE;

    float highest_charge = soc_stats->cell_stats[max_seg][max_SE].charge_c;

    // Find the index for the cell soc to look up the open-circuit voltage
    uint8_t lut_index;
    for (lut_index = 0U; lut_index < V_TO_SOC_LUT_SIZE; lut_index++)
    {
        if (energy_lookup[lut_index] < highest_charge)
        {
            break;
        }
    }

    return v_to_soc_lookup[lut_index];
}
