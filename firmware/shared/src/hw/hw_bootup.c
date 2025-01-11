#include "hw_bootup.h"
#include <stdlib.h>
#include "hw_hal.h"
#include "FreeRTOS.h"

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