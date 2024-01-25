#include "hw_bootup.h"
#include <stdlib.h>
#include "hw_hal.h"

// Defined in linker script.
extern uint32_t __app_code_start__;

void hw_bootup_enableInterruptsForApp()
{
    // Set vector table offset register.
    // The startup handler sets the VTOR to the default value (0x8000000), so even though we update it
    // in the bootloader, we have to update it again here.
    // (In SystemInit(), which is defined in system_stm32fXxx.c)
    SCB->VTOR = (uint32_t)&__app_code_start__;

    // We also need to re-enable interrupts since they are disabled by the bootloader before jumping to the app, to
    // prevent interrupts occuring mid-jump.
    __enable_irq();
}