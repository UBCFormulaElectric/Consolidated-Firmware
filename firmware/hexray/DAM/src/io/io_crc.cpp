#include "io_crc.hpp"

#include <array>
#include <cstring>

#include <FreeRTOS.h>
#include <semphr.h>

#include "hw_crc.hpp"
#include "hw_mutexGuard.hpp"

namespace io::crc
{

// The STM32 CRC peripheral is a single hardware state machine; concurrent
// HAL_CRC_Calculate calls (TX-side Header ctor on TaskCanRx, RX-side frame
// validation on TaskTelemRx) corrupt each other's results. Serialize here.
static StaticSemaphore_t s_mutex_storage;
static SemaphoreHandle_t s_mutex = nullptr;

void init()
{
    s_mutex = xSemaphoreCreateMutexStatic(&s_mutex_storage);
}

uint32_t calculatePayloadCrc(std::span<const uint8_t> payload)
{
    std::array<uint8_t, 256> crc_input{};
    memcpy(crc_input.data(), payload.data(), payload.size());

    const size_t crc_byte_count = payload.size();
    const auto   crc_bytes =
        std::span<const uint32_t>{ reinterpret_cast<const uint32_t *>(crc_input.data()), crc_byte_count };

    hw::MutexGuard g{ s_mutex };
    return hw::crc::calculate(crc_bytes);
}

} // namespace io::crc
