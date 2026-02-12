#include "hw_crc.hpp"

static CRC_HandleTypeDef *handle = nullptr;

namespace hw::crc
{
void init(CRC_HandleTypeDef *crc_handle)
{
    handle = crc_handle;
}

uint32_t calculate(const uint32_t *buffer, const uint32_t size_words)
{
    return HAL_CRC_Calculate(handle, const_cast<uint32_t *>(buffer), size_words);
}
} // namespace hw::crc