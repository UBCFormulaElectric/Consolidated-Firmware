#include "app_soc.h"
#include "app_math.h"
#include "app_tractiveSystem.h"
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


#define V_TO_SOC_LUT_SIZE (201U)
#define LUT_BASE_SOC (0.0f)
    
// Use the voltage to SoC lookup table to get the index. Use this index to look up the state of charge

// index 0 represents 0.0% soc, final index represents 100%. 0.5% increments.
static const float ocv_soc_lut[V_TO_SOC_LUT_SIZE] ={ 
    2.478484, 2.548948, 2.555629, 2.636801, 2.717973, 2.795096, 2.845899, 2.896701, 2.947503, 2.979689, 
    3.009237, 3.038785, 3.062674, 3.083212, 3.103750, 3.124092, 3.144124, 3.164156, 3.183431, 3.197409, 
    3.211386, 3.225364, 3.240896, 3.256680, 3.272465, 3.284931, 3.295543, 3.306156, 3.304352, 3.317410, 
    3.331939, 3.344696, 3.356658, 3.368619, 3.380526, 3.391995, 3.403385, 3.414583, 3.421610, 3.426887, 
    3.432164, 3.436899, 3.440721, 3.444543, 3.448370, 3.452317, 3.456320, 3.460323, 3.465373, 3.470832, 
    3.476372, 3.482455, 3.488869, 3.495323, 3.501739, 3.508092, 3.514425, 3.520623, 3.526513, 3.532403, 
    3.538241, 3.543478, 3.548635, 3.553792, 3.558754, 3.563582, 3.568411, 3.573168, 3.577760, 3.582344, 
    3.586892, 3.591139, 3.595281, 3.599423, 3.603309, 3.607144, 3.610956, 3.614661, 3.618294, 3.621917, 
    3.625508, 3.629068, 3.632628, 3.636199, 3.639814, 3.643430, 3.647046, 3.650764, 3.654502, 3.658240, 
    3.662069, 3.666019, 3.669968, 3.673983, 3.678140, 3.682339, 3.686552, 3.690932, 3.695370, 3.699882, 
    3.704528, 3.709202, 3.713828, 3.718485, 3.723182, 3.727879, 3.732598, 3.737347, 3.742095, 3.746847, 
    3.751637, 3.756426, 3.761216, 3.766030, 3.770867, 3.775704, 3.780582, 3.785519, 3.790488, 3.795490, 
    3.800562, 3.805651, 3.810753, 3.815940, 3.821127, 3.826319, 3.831529, 3.836744, 3.841952, 3.847058, 
    3.852135, 3.857211, 3.862209, 3.867022, 3.871836, 3.876640, 3.881274, 3.885880, 3.890486, 3.894997, 
    3.899466, 3.903929, 3.908409, 3.912903, 3.917498, 3.922166, 3.926984, 3.931814, 3.936749, 3.941997, 
    3.947246, 3.952528, 3.958148, 3.963841, 3.969533, 3.975359, 3.981300, 3.987241, 3.993173, 3.999034, 
    4.004879, 4.010713, 4.016239, 4.021650, 4.027055, 4.032055, 4.036909, 4.041577, 4.045854, 4.049776, 
    4.053605, 4.057009, 4.059732, 4.062456, 4.065076, 4.066936, 4.068797, 4.070658, 4.072325, 4.073932, 
    4.075540, 4.077227, 4.079082, 4.080939, 4.082910, 4.085239, 4.087742, 4.090254, 4.093629, 4.097167, 
    4.100969, 4.105953, 4.111315, 4.117198, 4.124902, 4.134033, 4.143165, 4.155217, 4.171970, 4.188723, 
    4.205476 };

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


