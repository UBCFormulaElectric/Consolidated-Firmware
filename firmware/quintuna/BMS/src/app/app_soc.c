#include "app_soc.h"
#include "app_math.h"
#include "app_tractiveSystem.h"
#include "app_canTx.h"
#include "io_sds.h"

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

#define V_TO_SOC_LUT_SIZE (201U)
#define LUT_BASE_SOC (0.0f)

// Use the voltage to SoC lookup table to get the index. Use this index to look up the state of charge

// index 0 represents 0.0% soc, final index represents 100%. 0.5% increments.
static const float ocv_soc_lut[V_TO_SOC_LUT_SIZE] = {
    2.478484f, 2.548948f, 2.555629f, 2.636801f, 2.717973f, 2.795096f, 2.845899f, 2.896701f, 2.947503f, 2.979689f,
    3.009237f, 3.038785f, 3.062674f, 3.083212f, 3.103750f, 3.124092f, 3.144124f, 3.164156f, 3.183431f, 3.197409f,
    3.211386f, 3.225364f, 3.240896f, 3.256680f, 3.272465f, 3.284931f, 3.295543f, 3.306156f, 3.304352f, 3.317410f,
    3.331939f, 3.344696f, 3.356658f, 3.368619f, 3.380526f, 3.391995f, 3.403385f, 3.414583f, 3.421610f, 3.426887f,
    3.432164f, 3.436899f, 3.440721f, 3.444543f, 3.448370f, 3.452317f, 3.456320f, 3.460323f, 3.465373f, 3.470832f,
    3.476372f, 3.482455f, 3.488869f, 3.495323f, 3.501739f, 3.508092f, 3.514425f, 3.520623f, 3.526513f, 3.532403f,
    3.538241f, 3.543478f, 3.548635f, 3.553792f, 3.558754f, 3.563582f, 3.568411f, 3.573168f, 3.577760f, 3.582344f,
    3.586892f, 3.591139f, 3.595281f, 3.599423f, 3.603309f, 3.607144f, 3.610956f, 3.614661f, 3.618294f, 3.621917f,
    3.625508f, 3.629068f, 3.632628f, 3.636199f, 3.639814f, 3.643430f, 3.647046f, 3.650764f, 3.654502f, 3.658240f,
    3.662069f, 3.666019f, 3.669968f, 3.673983f, 3.678140f, 3.682339f, 3.686552f, 3.690932f, 3.695370f, 3.699882f,
    3.704528f, 3.709202f, 3.713828f, 3.718485f, 3.723182f, 3.727879f, 3.732598f, 3.737347f, 3.742095f, 3.746847f,
    3.751637f, 3.756426f, 3.761216f, 3.766030f, 3.770867f, 3.775704f, 3.780582f, 3.785519f, 3.790488f, 3.795490f,
    3.800562f, 3.805651f, 3.810753f, 3.815940f, 3.821127f, 3.826319f, 3.831529f, 3.836744f, 3.841952f, 3.847058f,
    3.852135f, 3.857211f, 3.862209f, 3.867022f, 3.871836f, 3.876640f, 3.881274f, 3.885880f, 3.890486f, 3.894997f,
    3.899466f, 3.903929f, 3.908409f, 3.912903f, 3.917498f, 3.922166f, 3.926984f, 3.931814f, 3.936749f, 3.941997f,
    3.947246f, 3.952528f, 3.958148f, 3.963841f, 3.969533f, 3.975359f, 3.981300f, 3.987241f, 3.993173f, 3.999034f,
    4.004879f, 4.010713f, 4.016239f, 4.021650f, 4.027055f, 4.032055f, 4.036909f, 4.041577f, 4.045854f, 4.049776f,
    4.053605f, 4.057009f, 4.059732f, 4.062456f, 4.065076f, 4.066936f, 4.068797f, 4.070658f, 4.072325f, 4.073932f,
    4.075540f, 4.077227f, 4.079082f, 4.080939f, 4.082910f, 4.085239f, 4.087742f, 4.090254f, 4.093629f, 4.097167f,
    4.100969f, 4.105953f, 4.111315f, 4.117198f, 4.124902f, 4.134033f, 4.143165f, 4.155217f, 4.171970f, 4.188723f,
    4.205476f
};

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

#ifdef TARGET_EMBEDDED

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
    // TODO: uncommentt when can msg is added
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

static bool sdCardReady()
{
    return io_sds_checkSdPresent();
}

float app_soc_getMinOcvFromSoc(void)
{
    float soc_percent = app_soc_getMinSocPercent();
    return app_soc_getOcvFromSoc(soc_percent);
}

void app_soc_resetSocFromVoltage(void)
{
    // TODO: Change this back to app_accumulaotor call once app_acc is added
    // const float min_cell_voltage = app_accumulator_getMinCellVoltage(NULL, NULL);

    const float min_cell_voltage = 2.5f;
    const float soc_percent      = app_soc_getSocFromOcv(min_cell_voltage);

    // convert from percent to coulombs
    stats.charge_c = (double)(SERIES_ELEMENT_FULL_CHARGE_C * soc_percent / 100.0f);

    // Mark SOC as corrupt anytime SOC is reset
    stats.is_corrupt = true;
    // TODO: uncomment when can msg is added
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}

void app_soc_resetSocCustomValue(float soc_percent)
{
    stats.charge_c = (double)(soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);

    // Mark SOC as corrupt anytime SOC is reset
    stats.is_corrupt = true;
    // TODO: uncomment when can msg is added
    app_canTx_BMS_SocCorrupt_set(stats.is_corrupt);
}

bool app_soc_readSocFromSd(float *saved_soc_c)
{
    uint32_t sd_read_crc    = 0;
    float    soc            = 0.0f;
    uint32_t calculated_crc = 0;
    *saved_soc_c            = -1.0f;

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
