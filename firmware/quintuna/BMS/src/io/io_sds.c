#include "io_sds.h"
#include "hw_sds.h"
#include "hw_gpio.h"
#include "hw_crc.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>

#define SD_QUEUE_LENGTH 1

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

static osMessageQueueId_t sdcardQueueHandle = NULL;

void io_sds_queue_init(void)
{
    if (sdcardQueueHandle == NULL)
    {
        sdcardQueueHandle = osMessageQueueNew(SD_QUEUE_LENGTH, sizeof(SdRequest), NULL);
    }
}

bool io_sds_enqueue(SdRequest *req)
{
    if (sdcardQueueHandle == NULL)
        return false;
    return osMessageQueuePut(sdcardQueueHandle, req, 0, 0) == osOK;
}

osMessageQueueId_t io_sds_queue_handle(void)
{
    return sdcardQueueHandle;
}

bool io_sds_checkSdPresent(void)
{
    return !hw_gpio_readPin(sd1.present_gpio);
}

bool io_sds_readSocFromSd(float *saved_soc_c)
{
    uint32_t sd_read_crc    = 0;
    float    soc            = 0.0f;
    uint32_t calculated_crc = 0;
    *saved_soc_c            = -1.0f;

    uint8_t sd_read_crc_bytes[NUM_SOC_CRC_BYTES];
    uint8_t sd_read_soc_bytes[NUM_SOC_BYTES];
    uint8_t sd_read_data[SD_SECTOR_SIZE];

    uint32_t sd_read_soc;

    if (!io_sds_checkSdPresent())
    {
        return false;
    }

    if (hw_sd_read(sd_read_data, DEFAULT_SOC_ADDR, 1) == SD_CARD_OK)
    {
        memcpy(sd_read_soc_bytes, sd_read_data, sizeof(uint32_t));
        sd_read_soc = ARRAY_TO_UINT32(sd_read_soc_bytes);
        // Convert bytes to float
        memcpy(&soc, sd_read_soc_bytes, sizeof(float));
    }
    else
    {
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
        return false;
    }

    calculated_crc = hw_crc_calculate(&sd_read_soc, 1);

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

bool io_sds_writeSocToSd(float soc)
{
    if (!io_sds_checkSdPresent())
    {
        return false;
    }
    uint8_t sd_write_data[SD_SECTOR_SIZE];
    // Convert float to byte array
    memcpy(sd_write_data, (uint8_t *)(&soc), sizeof(float));
    uint32_t soc_value = ARRAY_TO_UINT32(sd_write_data);

    uint32_t crc_calculated = hw_crc_calculate(&soc_value, 1);
    uint8_t  crc_bytes[NUM_SOC_CRC_BYTES];
    UINT32_TO_ARRAY(crc_calculated, crc_bytes);

    hw_sd_write(sd_write_data, DEFAULT_SOC_ADDR, 1);

    // Write the CRC right after the SOC value
    hw_sd_write(crc_bytes, DEFAULT_SOC_ADDR + NUM_SOC_BYTES, 1);

    return true;
}
