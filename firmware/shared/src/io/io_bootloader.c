#include "io_bootloader.h"
#include "task.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include "main.h"
#include "hw_hal.h"
#include "io_log.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "hw_crc.h"
#include "app_commitInfo.h"

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;


_Noreturn static void modifyStackPointerAndStartApp(const uint32_t *address)
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
    __set_MSP(app_sp);
    void (*boot_reset_handler)(void) = (void (*)(void))boot_start;
    boot_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED()
    for (;;)
    {
    }
}

//MAY NOT NEED TO RUN VERIFYAPPCODECHECKSUM IF WE ARE JUMPING TO BOOTLOADER AS WE ALREADY TRUST THAT
//THE BOOTLOADER IS CORRECT AND WELL WRITTEN FOR FLASHING THE CODE ---> Comment out for now

// static BootStatus verifyAppCodeChecksum(void)
// {
//     if (*&___boot_code_start__ == 0xFFFFFFFF)
//     {
//         // If app initial stack pointer is all 0xFF, assume app is not present.
//         return BOOT_STATUS_NO_APP;
//     }

//     //TODO: Figure out how to configure boot_meta_data start in the linker script
//     Metadata *metadata = (Metadata *)&__app_metadata_start__;
//     if (metadata->size_bytes > (uint32_t)&__app_code_size__)
//     {
//         // App binary size field is invalid.
//         return BOOT_STATUS_APP_INVALID;
//     }

//     uint32_t calculated_checksum = hw_crc_calculate(&__app_code_start__, metadata->size_bytes / 4);
//     return calculated_checksum == metadata->checksum ? BOOT_STATUS_APP_VALID : BOOT_STATUS_APP_INVALID;
// }


void io_boot_JumpToBootCode(void)
{
    //recieve message from CANUP script to jump to bootloader

    //need to deintialize all the peripherals in which we configured in main.c
    modifyStackPointerAndStartApp(&__boot_code_start__);
}