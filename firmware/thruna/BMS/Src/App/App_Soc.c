#include <math.h>
#include "App_Soc.h"
#include <stdint.h>
#include <float.h>
#include "App_SharedConstants.h"
#include "App_SharedProcessing.h"
#include "lut/App_Lut_CellVoltageToSoc.h"

#define MS_TO_S (0.001f)
#define SOC_TIMER_DURATION (110U)

float App_Soc_GetSocFromOcv(float voltage)
{
    uint8_t lut_index = 0;

    while ((voltage > ocv_soc_lut[lut_index]) && (lut_index < V_TO_SOC_LUT_SIZE))
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

float App_Soc_GetOcvFromSoc(float soc_percent)
{
    uint8_t lut_index = 0;

    while ((LUT_BASE_SOC + lut_index * 0.5f < soc_percent) && (lut_index < V_TO_SOC_LUT_SIZE))
    {
        lut_index++;
    }

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        // Ensures that the index is in the LUT range
        lut_index--;
    }

    return ocv_soc_lut[lut_index];
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

struct SocStats *App_SocStats_Create(struct Eeprom *eeprom, struct Accumulator *accumulator)
{
    struct SocStats *soc_stats = malloc(sizeof(struct SocStats));
    assert(soc_stats != NULL);
    soc_stats->prev_current_A = 0.0f;
    soc_stats->soc_address    = DEFAULT_SOC_ADDR;

    // Asoc assumed corrupt until proven otherwise
    soc_stats->is_corrupt = true;
    soc_stats->charge_c   = -1;

    // A negative soc value will indicate to App_SocStats_Create that saved SOC value is corrupted
    float saved_soc_c = -1.0f;
    if (App_Eeprom_ReadSocAddress(eeprom, &soc_stats->soc_address) == EXIT_CODE_OK)
    {
        if (App_Eeprom_ReadMinSoc(eeprom, soc_stats->soc_address, &saved_soc_c) == EXIT_CODE_OK)
        {
            if (IS_IN_RANGE(0.0f, SERIES_ELEMENT_FULL_CHARGE_C * 1.25f, saved_soc_c))
            {
                soc_stats->charge_c   = (double)saved_soc_c;
                soc_stats->is_corrupt = false;
            }
        }
    }
    else
    {
        // If address corrupted, revert to default SOC address location
        soc_stats->soc_address = DEFAULT_SOC_ADDR;
    }

    // Update the active address that SOC is stored at
    if (App_Eeprom_UpdateSavedSocAddress(eeprom, &soc_stats->soc_address) != EEPROM_OK)
    {
        soc_stats->soc_address = DEFAULT_SOC_ADDR;
    }
    App_Timer_InitTimer(&soc_stats->soc_timer, SOC_TIMER_DURATION);
    App_CanTx_BMS_SocCorrupt_Set(soc_stats->is_corrupt);

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

bool App_SocStats_GetCorrupt(struct SocStats *soc_stats)
{
    return soc_stats->is_corrupt;
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

float App_SocStats_GetMinSocCoulombs(struct SocStats *soc_stats)
{
    // return SOC in Coulombs
    return (float)soc_stats->charge_c;
}

float App_SocStats_GetMinSocPercent(struct SocStats *soc_stats)
{
    // return SOC in %
    float soc_percent = ((float)soc_stats->charge_c / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
    return soc_percent;
}

float App_Soc_GetMinOcvFromSoc(struct SocStats *soc_stats)
{
    float soc_percent = App_SocStats_GetMinSocPercent(soc_stats);
    return App_Soc_GetOcvFromSoc(soc_percent);
}

void App_Soc_ResetSocFromVoltage(struct SocStats *soc_stats, struct Accumulator *accumulator)
{
    uint8_t segment;
    uint8_t cell;

    const float min_cell_voltage = App_Accumulator_GetMinVoltage(accumulator, &segment, &cell);

    const float soc_percent = App_Soc_GetSocFromOcv(min_cell_voltage);

    // convert from percent to coulombs
    soc_stats->charge_c = (double)(SERIES_ELEMENT_FULL_CHARGE_C * soc_percent / 100.0f);

    // Mark SOC as corrupt anytime SOC is reset
    soc_stats->is_corrupt = true;
    App_CanTx_BMS_SocCorrupt_Set(soc_stats->is_corrupt);
}

void App_Soc_ResetSocCustomValue(struct SocStats *soc_stats, float soc_percent)
{
    soc_stats->charge_c = (double)(soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);

    // Mark SOC as corrupt anytime SOC is reset
    soc_stats->is_corrupt = true;
    App_CanTx_BMS_SocCorrupt_Set(soc_stats->is_corrupt);
}
