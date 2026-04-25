#pragma once

#include <stdint.h>

#ifndef BOOTLOAD
#error \
    "This header is only meant to be included in the bootloader, since it relies on the .boot_request section being at the end of the stack. If you want to use this in the app, you'll need to move the .boot_request section somewhere else and update the linker script accordingly."
#endif

// What you want to boot into next.
typedef enum __attribute__((packed))
{
    BOOT_TARGET_APP,
    BOOT_TARGET_BOOTLOADER,
} BootTarget;

// Any extra information associated with a boot.
typedef enum __attribute__((packed))
{
    BOOT_CONTEXT_NONE,
    BOOT_CONTEXT_STACK_OVERFLOW,
    BOOT_CONTEXT_WATCHDOG_TIMEOUT,
} BootContext;

typedef struct
{
    BootTarget  target;
    BootContext context;
    uint16_t    _unused;
    uint32_t    context_value; // Context-dependent value
} BootRequest;

/**
 * Due to quirks of STM32's generated code, this **must** be invoked at the top of main.c if using our CAN bootloader.
 * Otherwise interrupts will not work.
 */
void hw_bootup_enableInterruptsForApp(void);

void hw_bootup_setBootRequest(BootRequest request);

BootRequest hw_bootup_getBootRequest(void);
