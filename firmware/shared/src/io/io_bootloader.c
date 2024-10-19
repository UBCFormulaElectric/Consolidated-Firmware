#include "io_bootloader.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include "main.h"
#include "hw_hal.h"
#include "io_log.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "app_commitInfo.h"

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;

static void modifyStackPointerAndStartApp(const uint32_t *address)
{
    // Disable interrupts before jumping.
    __disable_irq();

    // Disable system tick to stop FreeRTOS timebase.
    SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;

    // Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
    // to 0xFFFFFFFF. This is done so no interrupts are queued up when we jump to the app.
    // (There are 8 registers on the Cortex-M4)
    for (uint32_t i = 0; i < 8; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Update the vector table offset register. When an interrupt is fired,
    // the microcontroller looks for the corresponding interrupt service handler
    // at the memory address in the VTOR. We need to update it so the app ISRs
    // are used.
    SCB->VTOR = (uint32_t)address;

    // Flush processor pipeline.
    __ISB();

    // Tell MCU to use the main stack pointer rather than process stack pointer (PSP is used with RTOS)
    __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);

    // Modify stack pointer and jump to app code.
    // In a binary built with our linker settings, the interrupt vector table is the first thing
    // placed in flash. The first word of the vector table contains the initial stack pointer
    // and the second word containers the address of the reset handler. Update stack pointer and
    // program counter accordingly.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    uint32_t boot_sp    = address[0];
    uint32_t boot_start = address[1];
#pragma GCC diagnostic pop
    __set_MSP(boot_sp);
    void (*boot_reset_handler)(void) = (void (*)(void))boot_start;
    boot_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED()
    for (;;)
    {
    }
}

void io_boot_jumpToBootCode()
{
    modifyStackPointerAndStartApp(&__boot_code_start__);
}
