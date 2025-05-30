#include "hw_bootup.h"
#include "main.h"
#include "io_bootHandler.h"
#include "io_log.h"
#include "bootloaderConfig.h"
#include <stdint.h>

#define BOOT_CAN_START_LOWBITS 0x9

void io_bootHandler_processBootRequest(const CanMsg *msg)
{
    if (msg->std_id == (BOARD_HIGHBITS | BOOT_CAN_START_LOWBITS))
    {
        LOG_INFO("Received CAN message, entering bootloader");
        hw_bootup_setBootRequest(BOOT_REQUEST_BOOTLOADER);
        for (uint8_t i = 0; i < 100; i++)
        {
            __ASM("nop");
        }
        NVIC_SystemReset();
    }
}
