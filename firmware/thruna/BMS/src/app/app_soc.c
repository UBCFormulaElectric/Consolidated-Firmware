#include <math.h>
#include "app_soc.h"
#include <stdint.h>
#include <float.h>
#include "app_math.h"
#include "lut/app_cellVoltageToSocLut.h"
#include "app_tractiveSystem.h"

#define MS_TO_S (0.001)
#define SOC_TIMER_DURATION (110U)

typedef struct
{
    // Address in EEPROM where SOC is being stored (address changes for wear levelling reasons, address always stored in
    // address 0 of EEPROM)
    uint16_t soc_address;

    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    // Indicates if SOC from EEPROM was corrupt at startup
    bool is_corrupt;

    TimerChannel soc_timer;
} SocStats;

static SocStats stats;

#ifndef TARGET_EMBEDDED

void app_soc_setPrevCurrent(float current)
{
    stats.prev_current_A = current;
}

#endif

float app_soc_getSocFromOcv(float voltage)
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

float app_soc_getOcvFromSoc(float soc_percent)
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

void app_soc_init(void)
{
    stats.prev_current_A = 0.0f;
    stats.soc_address    = DEFAULT_SOC_ADDR;

    // Asoc assumed corrupt until proven otherwise
    stats.is_corrupt = true;
    stats.charge_c   = -1;

    // A negative soc value will indicate to app_soc_Create that saved SOC value is corrupted
    float saved_soc_c = -1.0f;
    if (app_eeprom_readSocAddress(&stats.soc_address) == EXIT_CODE_OK)
    {
        if (app_eeprom_readMinSoc(stats.soc_address, &saved_soc_c) == EXIT_CODE_OK)
        {
            if (IS_IN_RANGE(0.0f, SERIES_ELEMENT_FULL_CHARGE_C * 1.25f, saved_soc_c))
            {
                stats.charge_c   = (double)saved_soc_c;
                stats.is_corrupt = false;
            }
        }
    }
    else
    {
        // If address corrupted, revert to default SOC address location
        stats.soc_address = DEFAULT_SOC_ADDR;
    }

    // Update the active address that SOC is stored at
    if (app_eeprom_updateSavedSocAddress(&stats.soc_address) != EEPROM_STATUS_OK)
    {
        stats.soc_address = DEFAULT_SOC_ADDR;
    }

    app_timer_init(&stats.soc_timer, SOC_TIMER_DURATION);
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}

uint16_t app_soc_getSocAddress(void)
{
    return stats.soc_address;
}

bool app_soc_getCorrupt(void)
{
    return stats.is_corrupt;
}

void app_soc_updateSocStats(void)
{
    // NOTE current sign is relative to current into the battery
    double *charge_c     = &stats.charge_c;
    float  *prev_current = &stats.prev_current_A;
    float   current      = app_tractiveSystem_getCurrent();

    double elapsed_time_s = (double)app_timer_getElapsedTime(&stats.soc_timer) * MS_TO_S;
    app_timer_restart(&stats.soc_timer);

    // Trapezoidal Rule adds integral of current time-step to previous integral value.
    app_math_trapezoidalRule(charge_c, prev_current, current, elapsed_time_s);
}

float app_soc_getMinSocCoulombs(void)
{
    // return SOC in Coulombs
    return (float)stats.charge_c;
}

float app_soc_getMinSocPercent(void)
{
    // return SOC in %
    float soc_percent = ((float)stats.charge_c / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
    return soc_percent;
}

float app_soc_getMinOcvFromSoc(void)
{
    float soc_percent = app_soc_getMinSocPercent();
    return app_soc_getOcvFromSoc(soc_percent);
}

void app_soc_resetSocFromVoltage(void)
{
    const float min_cell_voltage = app_accumulator_getMinCellVoltage(NULL, NULL);
    const float soc_percent      = app_soc_getSocFromOcv(min_cell_voltage);

    // convert from percent to coulombs
    stats.charge_c = (double)(SERIES_ELEMENT_FULL_CHARGE_C * soc_percent / 100.0f);

    // Mark SOC as corrupt anytime SOC is reset
    stats.is_corrupt = true;
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}

void app_soc_resetSocCustomValue(float soc_percent)
{
    stats.charge_c = (double)(soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);

    // Mark SOC as corrupt anytime SOC is reset
    stats.is_corrupt = true;
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}
