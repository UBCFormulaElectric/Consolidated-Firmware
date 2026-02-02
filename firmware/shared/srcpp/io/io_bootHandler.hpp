#pragma once

#include "hw_bootup.hpp"
#include "main.h"
#include "io_log.hpp"
#include "bootloaderConfig.h"
#include <cstdint>
#include <io_canMsg.hpp>

#define BOOT_CAN_START_LOWBITS 0x9

namespace io::bootHandler
{
    
inline void processBootRequest(const io::CanMsg &msg)
{
    if (msg.std_id == (BOARD_HIGHBITS | BOOT_CAN_START_LOWBITS))
    {
        LOG_INFO("Received CAN message, entering bootloader");

        const hw::bootup::BootRequest request = { .target        = hw::bootup::BootTarget::BOOT_TARGET_BOOTLOADER,
                                                  .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
                                                  .context_value = 0 };
        hw::bootup::setBootRequest(request);

        // Empirically need to spin for a few cycles or the boot request doesn't get written properly before reset.
        for (uint8_t i = 0; i < 100; i++)
        {
            __ASM("nop");
        }

        NVIC_SystemReset();
        forever;
    }
}
} // namespace io::bootHandler