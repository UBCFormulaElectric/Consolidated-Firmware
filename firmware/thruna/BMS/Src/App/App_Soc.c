#include <math.h>
#include "App_Soc.h"
#include <stdint.h>
#include <float.h>
#include "App_SharedConstants.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define STATE_OF_HEALTH (1.0f)
#define SERIES_ELEMENT_FULL_CHARGE_C (5.9f * 3600.0f * 3.0f * STATE_OF_HEALTH)
#define MS_TO_S (0.001f)
#define SOC_TIMER_DURATION (100U)

float App_Soc_GetSocFromVoc(float voltage)
{
    uint8_t lut_index = 0;

    while( (voltage > voc_soc_lut[lut_index]) && (lut_index < V_TO_SOC_LUT_SIZE))
    {
        lut_index++;
    }

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        lut_index--;
    }
    return LUT_BASE_SOC + lut_index * 0.5f;
}

float App_Soc_GetVocFromSoc(float soc_percent)
{
    uint8_t lut_index = 0;

    while( (LUT_BASE_SOC + lut_index * 0.5f < soc_percent) && (lut_index < V_TO_SOC_LUT_SIZE))
    {
        lut_index++;
    }

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
    // Ensures that the index is in the LUT range
    lut_index--;
    }

    return voc_soc_lut[lut_index];
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
        soc_stats->charge_c = (double)initial_charge_value;
    }

    App_Timer_InitTimer(&soc_stats->soc_timer, SOC_TIMER_DURATION);

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

void App_SocStats_UpdateSocStats(struct SocStats *soc_stats, float current)
{
    // NOTE current sign is relative to current into the battery
    double *charge_c     = &soc_stats->charge_c;
    float * prev_current = &soc_stats->prev_current_A;

    static uint32_t times_run = 0;
    times_run++;
    UNUSED(times_run);

    float elapsed_time_s = (float)App_Timer_GetElapsedTime(&soc_stats->soc_timer) * MS_TO_S;
    App_Timer_Restart(&soc_stats->soc_timer);

    // Trapezoidal Rule adds integral of current time-step to previous integral value.
    App_SharedProcessing_TrapezoidalRule(charge_c, prev_current, current, elapsed_time_s);
}

float App_SocStats_GetMinSoc(struct SocStats *soc_stats)
{
    // return SOC in %
    return ((float)soc_stats->charge_c / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
}

float App_SOC_GetMinVocFromSoc(struct SocStats *soc_stats)
{
    float soc_percent = App_SocStats_GetMinSoc(soc_stats);
    return App_Soc_GetVocFromSoc(soc_percent);
}

void App_SOC_ResetSocFromVoltage(struct SocStats *soc_stats, struct Accumulator *accumulator)
{
    uint8_t segment;
    uint8_t cell;

    const float min_cell_voltage = App_Accumulator_GetMinVoltage(accumulator, &segment, &cell);

    float soc_percent = App_Soc_GetSocFromVoc(min_cell_voltage);

    // convert from percent to coulombs
    soc_stats->charge_c = (double)(soc_percent * SERIES_ELEMENT_FULL_CHARGE_C);
}

void App_SOC_ResetSocCustomValue(struct SocStats *soc_stats, float soc_percent)
{
    soc_stats->charge_c = (double)((soc_percent / 100.0f) * SERIES_ELEMENT_FULL_CHARGE_C);
}