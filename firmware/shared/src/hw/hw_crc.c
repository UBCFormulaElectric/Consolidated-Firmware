#include "hw_crc.h"

static CRC_HandleTypeDef * handle;

void hw_crc_init(CRC_HandleTypeDef * crc_handle)
{
    handle = crc_handle;
}

uint32_t hw_crc_calculate(uint32_t * buffer, uint32_t size_words)
{
    return HAL_CRC_Calculate(handle, buffer, size_words);
}