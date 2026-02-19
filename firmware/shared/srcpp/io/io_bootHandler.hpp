#pragma once

#include "hw_bootup.hpp"
#include "main.h"
#include "io_log.hpp"
#include "cmsis_gcc.h"
#include <cstdint>
#include "hw_can.hpp"
#include "bootloader_h5.hpp"

#define BOOT_CAN_START_LOWBITS 0x9

namespace io::bootHandler
{

static void processBootRequest(const hw::CanMsg &msg)
{
    if (msg.std_id == (BOARD_HIGHBITS | BOOT_CAN_START_LOWBITS))
    {
        LOG_INFO("Received CAN message, entering bootloader");

        const hw::bootup::BootRequest request = { .target        = hw::bootup::BootTarget::BOOT_TARGET_BOOTLOADER,
                                                  .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
                                                  ._unused       = 0xFFFF,
                                                  .context_value = 0 };
        hw::bootup::setBootRequest(request);

        __DSB();
        __ISB();

        NVIC_SystemReset();
        forever;
    }
}
} // namespace io::bootHandler