#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BOOT_REQUEST_APP,
    BOOT_REQUEST_BOOTLOADER,
} BootRequest;

/**
 * Due to quirks of STM32's generated code, this **must** be invoked at the top of main.c if using our CAN bootloader.
 * Otherwise interrupts will not work.
 */
void hw_bootup_enableInterruptsForApp(void);

void hw_bootup_setBootRequest(BootRequest request);

BootRequest hw_bootup_getBootRequest(void);
