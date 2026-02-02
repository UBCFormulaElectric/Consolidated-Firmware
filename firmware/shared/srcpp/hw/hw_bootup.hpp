#pragma once

#include <cstdint>
#include <cstdbool>
#include "hw_hal.hpp"
#include "FreeRTOS.h"

// SRAM state at power-on is unpredictable - checking for the presence of a "magic number" is a quick-and-dirty way to
// check if the boot request has been intentionally written or not.
constexpr uint32_t BOOT_MAGIC = 0xAB;

// Defined in linker script.
extern uint32_t __app_code_start__;

namespace hw::bootup
{

enum class BootTarget : uint8_t
{
    BOOT_TARGET_APP,
    BOOT_TARGET_BOOTLOADER,
};

enum class BootContext : uint8_t
{
    BOOT_CONTEXT_NONE,
    BOOT_CONTEXT_STACK_OVERFLOW,
    BOOT_CONTEXT_WATCHDOG_TIMEOUT,
};

// Note the ordering matters as given the system is 4-byte aligned
// It is important that the uint16_t _unused is between the two uint8_t fields
// to account for padding and ensure consistent struct size across compilers
struct BootRequest
{
    BootTarget  target;
    BootContext context;
    uint16_t    _unused;
    uint32_t    context_value; // Context-dependent value
};
struct BootRequestData
{
    uint32_t    magic;
    BootRequest request;
};

// Boot flag from RAM
volatile __attribute__((section(".boot_request"))) BootRequestData boot_request;

/**
 * Due to quirks of STM32's generated code, this **must** be invoked at the top of main.c if using our CAN bootloader.
 * Otherwise interrupts will not work.
 */
inline void enableInterruptsForApp()
{
    // Set vector table offset register.
    // The startup handler sets the VTOR to the default value (0x8000000), so even though we update it
    // in the bootloader, we have to update it again here.
    // (In SystemInit(), which is defined in system_stm32fXxx.c)
    SCB->VTOR = (uint32_t)&__app_code_start__;

    // We also need to re-enable interrupts since they are disabled by the bootloader before jumping to the app, to
    // prevent interrupts occuring mid-jump.
    __enable_irq();

    // Any RTOS functions called in the bootloader before its RTOS is started will break interrupts. RTOS functions
    // invoke enter/exit critical to provide thread safety, which essentially just disables interrupts. FreeRTOS also
    // provides critical section nesting, which is kept track of in a static variable. However, this static variable is
    // initialized to a seemingly random large value before the RTOS starts. This means that if we call an RTOS
    // function, the critical section exit logic doesn't run, and interrupts aren't re-enabled.
    //
    // So: If any RTOS code runs in the bootloader before its RTOS starts (even initialization stuff), interrupts
    // are broken until the scheduler starts. So, re-enable interrupts here in case that happened, since the app we're
    // jumping too might rely on interrupts in initialization code (like for HAL_Delay, for example).
    portENABLE_INTERRUPTS();
}

inline void setBootRequest(BootRequest request)
{
    boot_request.magic                 = BOOT_MAGIC;
    boot_request.request.target        = request.target;
    boot_request.request.context       = request.context;
    boot_request.request._unused       = request._unused;
    boot_request.request.context_value = request.context_value;
}

inline BootRequest getBootRequest()
{
    if (boot_request.magic == BOOT_MAGIC)
    {
        BootRequest r;
        r.target        = boot_request.request.target;
        r.context       = boot_request.request.context;
        r._unused       = boot_request.request._unused;
        r.context_value = boot_request.request.context_value;

        return r;
    }
    else
    {
        // Default to app if magic not present.
        const BootRequest request = {
            .target        = BootTarget::BOOT_TARGET_APP,
            .context       = BootContext::BOOT_CONTEXT_NONE,
            .context_value = 0,
        };
        return request;
    }
}
} // namespace hw::bootup