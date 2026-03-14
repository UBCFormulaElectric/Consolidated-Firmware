#include "hw_crc.hpp"
#include "main.h"
#include <span>

namespace hw::crc
{

uint32_t calculate(std::span<const uint32_t> buffer_words)
{
    return HAL_CRC_Calculate(
        &hcrc, const_cast<uint32_t *>(buffer_words.data()), static_cast<uint32_t>(buffer_words.size()));
}
} // namespace hw::crc