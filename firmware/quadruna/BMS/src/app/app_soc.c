#include "app_soc.h"
#include "app_math.h"
#include "app_tractiveSystem.h"
#include "lut/app_cellVoltageToSocLut.h"
#include "io_sd.h"

#ifdef TARGET_EMBEDDED
#include "hw_sd.h"
#include "hw_crc.h"
#endif

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

#ifdef TARGET_EMBEDDED

static void convert_float_to_bytes(uint8_t *byte_array, float float_to_convert)
{
    memcpy(byte_array, (uint8_t *)(&float_to_convert), sizeof(float));
}

static float convert_bytes_to_float(uint8_t *byte_array)
{
    float converted_float;
    memcpy(&converted_float, byte_array, sizeof(float));
    return converted_float;
}

#else

// ONLY FOR USE IN OFF-TARGET TESTING
void app_soc_setPrevCurrent(float current)
{
    stats.prev_current_A = current;
}

#endif

static bool sdCardReady(void)
{
    return io_sdGpio_checkSdPresent();
}

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

#ifdef TARGET_EMBEDDED
    // A negative SOC value will indicate to app_soc_Create that saved SOC value is corrupted
    float saved_soc_c = -1.0f;

    if (app_soc_readSocFromSd(&saved_soc_c))
    {
        if (IS_IN_RANGE(0.0f, SERIES_ELEMENT_FULL_CHARGE_C * 1.25f, saved_soc_c))
        {
            stats.charge_c   = (double)saved_soc_c;
            stats.is_corrupt = false;
        }
    }

#endif

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

bool app_soc_readSocFromSd(float *saved_soc_c)
{
    uint32_t sd_read_crc = 0;
    float    soc = 0.0f;
    uint32_t calculated_crc = 0;
    *saved_soc_c = -1.0f;

    if (!sdCardReady())
    {
        return false;
    }

#ifdef TARGET_EMBEDDED
    uint8_t sd_read_crc_bytes[NUM_SOC_CRC_BYTES];
    uint8_t sd_read_soc_bytes[NUM_SOC_BYTES];
    uint8_t sd_read_data[SD_SECTOR_SIZE];

    uint32_t sd_read_soc;

    if (hw_sd_read(sd_read_data, DEFAULT_SOC_ADDR, 1) == SD_CARD_OK)
    {
        memcpy(sd_read_soc_bytes, sd_read_data, sizeof(uint32_t));
        sd_read_soc = ARRAY_TO_UINT32(sd_read_soc_bytes);
        soc         = convert_bytes_to_float(sd_read_soc_bytes);
    }
    else
    {
        *saved_soc_c = -1.0f;
        return false;
    }

    // CRC bytes are stored right after the SOC bytes
    if (hw_sd_read(sd_read_data, DEFAULT_SOC_ADDR + NUM_SOC_BYTES, 1) == SD_CARD_OK)
    {
        memcpy(sd_read_crc_bytes, sd_read_data, sizeof(uint32_t));
        sd_read_crc = ARRAY_TO_UINT32(sd_read_crc_bytes);
    }
    else
    {
        *saved_soc_c = -1.0f;
        return false;
    }

    calculated_crc = hw_crc_calculate(&sd_read_soc, 1);

#endif

    if (calculated_crc == sd_read_crc)
    {
        *saved_soc_c = soc;
        return true;
    }
    else
    {
        return false;
    }
}

bool app_soc_writeSocToSd(float soc)
{
    if (!sdCardReady())
    {
        return false;
    }

#ifdef TARGET_EMBEDDED
    uint8_t sd_write_data[SD_SECTOR_SIZE];
    convert_float_to_bytes(sd_write_data, soc);
    uint32_t soc_value = ARRAY_TO_UINT32(sd_write_data);

    uint32_t crc_calculated = hw_crc_calculate(&soc_value, 1);
    uint8_t  crc_bytes[4];
    UINT32_TO_ARRAY(crc_calculated, crc_bytes);

    hw_sd_write(sd_write_data, DEFAULT_SOC_ADDR, 1);
    hw_sd_write(crc_bytes, DEFAULT_SOC_ADDR + 4, 1);

#endif
    return true;
}
