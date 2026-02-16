#pragma once

#include "hw_bootup.hpp"
#include "main.h"
#include "io_log.hpp"
#include "cmsis_gcc.h"
#include <cstdint>
#include "io_canMsg.hpp"

#define BOOT_CAN_START_LOWBITS 0x9
extern uint32_t BOARD_HIGHBITS;

namespace io::bootHandler
{

static void processBootRequest(const io::CanMsg &msg)
{
    if (msg.std_id == (2 | BOOT_CAN_START_LOWBITS))
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