#include "hw_bootup.h"
#include "main.h"
#include "io_bootHandler.h"
#include "io_log.h"
#include "bootloaderConfig.h"

#define BOOT_CAN_START_LOWBITS 0x9

void io_bootHandler_processBootRequest(const CanMsg *msg)
{
    if (msg->std_id == (BOARD_HIGHBITS | BOOT_CAN_START_LOWBITS))
    {
        LOG_INFO("Received CAN message, entering bootloader");
        const BootRequest request = { .target        = BOOT_TARGET_BOOTLOADER,
                                      .context       = BOOT_CONTEXT_NONE,
                                      .context_value = 0 };
        hw_bootup_setBootRequest(request);
        NVIC_SystemReset();
    }
}
