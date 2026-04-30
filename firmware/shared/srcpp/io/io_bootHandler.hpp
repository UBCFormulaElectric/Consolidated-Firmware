#pragma once

#include "hw_bootup.hpp"
#include "io_log.hpp"
#include "cmsis_gcc.h"
#include "hw_can.hpp"
#include "hw_error.hpp"

constexpr uint8_t BOOT_CAN_START_LOWBITS = 0x9;
namespace io::bootHandler
{
inline void processBootRequest(const hw::CanMsg &msg, const uint32_t board_highbits)
{
    if (msg.std_id == (board_highbits | BOOT_CAN_START_LOWBITS))
    {
        LOG_INFO("Received CAN message, entering bootloader");
        hw::bootup::setBootRequest({ .target        = hw::bootup::BootTarget::BOOT_TARGET_BOOTLOADER,
                                     .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
                                     ._unused       = 0xFFFF,
                                     .context_value = 0 });

        __DSB();
        __ISB();

        NVIC_SystemReset();
        Error_Handler();
    }
}
} // namespace io::bootHandler