#include <math.h>
#include "App_Soc.h"
#include <stdint.h>
#include <float.h>
#include "App_SharedConstants.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define STATE_OF_HEALTH (1.0f)
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

struct SocStats *App_SocStats_Create(float initial_charge_value, uint16_t soc_address, struct Accumulator *accumulator)
{
    struct SocStats *soc_stats = malloc(sizeof(struct SocStats));
    assert(soc_stats != NULL);
    soc_stats->prev_current_A = 0.0f;

    // RESETTING ON INSTANTIATION, UPDATE THIS WITH EEPROM VALUES IF AVAILABLE
    soc_stats->soc_address = soc_address;

    // input a negative initial value if EEPROM reading corrupted. Reset SOC values based on cell voltages.
    if (initial_charge_value < 0)
    {
        App_SOC_ResetSocFromVoltage(soc_stats, accumulator);
    }
    else
    {
        soc_stats->charge_c = initial_charge_value;
    }

    return soc_stats;
}

void App_SocStats_Destroy(struct SocStats *soc_stats)
{
    free(soc_stats);
}

uint16_t App_SocStats_GetSocAddress(struct SocStats *soc_stats)
{
    return soc_stats->soc_address;
}

void App_SocStats_UpdateSocStats(struct SocStats *soc_stats, float current, float time_step_s)
{
    static int callcount = 0;
    // NOTE current sign is relative to current into the battery
    double *charge_c     = &soc_stats->charge_c;
    float * prev_current = &soc_stats->prev_current_A;

    // Trapezoidal Rule adds integral of current time-step to previous integral value.
    App_SharedProcessing_TrapezoidalRule(charge_c, prev_current, current, time_step_s);
    callcount++;
    UNUSED(callcount);
}

float App_SocStats_GetMinSoc(struct SocStats *soc_stats)
{
    // return SOC in %
    return ((float)soc_stats->charge_c / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
}

float App_SOC_GetMinVocFromSoc(struct SocStats *soc_stats)
{
    float charge_c = (float)soc_stats->charge_c;

    uint8_t lut_index;

    // Find the index for the cell soc to look up the open-circuit voltage
    for (lut_index = 0U; charge_c < energy_lookup[lut_index]; lut_index++)
        ;

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        lut_index--;
    }

    return v_to_soc_lookup[lut_index];
}

void App_SOC_ResetSocFromVoltage(struct SocStats *soc_stats, struct Accumulator *accumulator)
{
    uint8_t segment;
    uint8_t cell;

    const float cell_voltage = App_Accumulator_GetMinVoltage(accumulator, &segment, &cell);

    uint8_t lut_index;
    for (lut_index = 0; cell_voltage < v_to_soc_lookup[lut_index]; lut_index++)
        ;

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        lut_index--;
    }

    // Divide by 1000 for mA to A conversion
    soc_stats->charge_c = energy_lookup[lut_index] / 1000;
}

void App_SOC_ResetSocCustomValue(struct SocStats *soc_stats, float soc_percent)
{
    soc_stats->charge_c = (soc_percent / 100.0f) * SERIES_ELEMENT_FULL_CHARGE_C;
}