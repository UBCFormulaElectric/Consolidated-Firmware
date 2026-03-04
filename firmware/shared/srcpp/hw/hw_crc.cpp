#include "hw_crc.hpp"

// Set during board startup via hw::crc::init(...) before any calculate() calls.
static CRC_HandleTypeDef *handle = nullptr;

namespace hw::crc
{
void init(CRC_HandleTypeDef *crc_handle)
{
    handle = crc_handle;
}

uint32_t calculate(std::span<const uint32_t> buffer_words)
{
    return HAL_CRC_Calculate(
        handle, const_cast<uint32_t *>(buffer_words.data()), static_cast<uint32_t>(buffer_words.size()));
}
} // namespace hw::crc