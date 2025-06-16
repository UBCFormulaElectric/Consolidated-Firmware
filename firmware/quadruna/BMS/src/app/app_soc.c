#include "app_soc.h"
#include "app_math.h"
#include "app_tractiveSystem.h"
#include "lut/app_cellVoltageToSocLut.h"
// #include "io_sd.h"

// #ifdef TARGET_EMBEDDED
// #include "hw_sd.h"
// #include "hw_crc.h"
// #endif

#include <stdint.h>
#include <float.h>
#include <string.h>
#include <math.h>

#define MS_TO_S (0.001)
#define SOC_TIMER_DURATION (110U)

#define NUM_SOC_BYTES (4U)
#define NUM_SOC_CRC_BYTES (4U)

#define DEFAULT_SOC_ADDR (0U)
#define SD_SECTOR_SIZE (512)
// Macro to convert a uint8_t array to a uint32_t variable
#define ARRAY_TO_UINT32(array) \
    (((uint32_t)(array)[0] << 24) | ((uint32_t)(array)[1] << 16) | ((uint32_t)(array)[2] << 8) | ((uint32_t)(array)[3]))

// Macro to convert a uint32_t variable to a uint8_t array
#define UINT32_TO_ARRAY(value, array)          \
    do                                         \
    {                                          \
        (array)[0] = (uint8_t)((value) >> 24); \
        (array)[1] = (uint8_t)((value) >> 16); \
        (array)[2] = (uint8_t)((value) >> 8);  \
        (array)[3] = (uint8_t)(value);         \
    } while (0)

typedef struct
{
    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    // Indicates if SOC from SD was corrupt at startup
    bool is_corrupt;

    TimerChannel soc_timer;
} SocStats;

static SocStats stats;
extern bool     sd_inited;

#ifndef TARGET_EMBEDDED

// ONLY FOR USE IN OFF-TARGET TESTING
void app_soc_setPrevCurrent(const float current)
{
    stats.prev_current_A = current;
}

#endif

float app_soc_getSocFromOcv(const float voltage)
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

float app_soc_getOcvFromSoc(const float soc_percent)
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

    // SOC assumed corrupt until proven otherwise
    stats.is_corrupt = true;
    stats.charge_c   = -1;

    // A negative SOC value will indicate to app_soc_Create that saved SOC value is corrupted
    float saved_soc_c = -1.0f;

    app_timer_init(&stats.soc_timer, SOC_TIMER_DURATION);
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}

bool app_soc_getCorrupt(void)
{
    return stats.is_corrupt;
}

void app_soc_updateSocStats(void)
{
    // NOTE current sign is relative to current into the battery
    double     *charge_c     = &stats.charge_c;
    float      *prev_current = &stats.prev_current_A;
    const float current      = app_tractiveSystem_getCurrent();

    const double elapsed_time_s = (double)app_timer_getElapsedTime(&stats.soc_timer) * MS_TO_S;
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
    const float soc_percent = ((float)stats.charge_c / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
    return soc_percent;
}

float app_soc_getMinOcvFromSoc(void)
{
    const float soc_percent = app_soc_getMinSocPercent();
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

void app_soc_resetSocCustomValue(const float soc_percent)
{
    stats.charge_c = (double)(soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);

    // Mark SOC as corrupt anytime SOC is reset
    stats.is_corrupt = true;
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}
