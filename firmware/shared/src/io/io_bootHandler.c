#include "io_bootHandler.h"
#include "bootloaderConfig.h"

// hw:
#include "hw_hal.h"

// io:
#include "io_log.h"

#include <hw_utils.h>

#define BOOT_CAN_START_LOWBITS 0x9

__attribute__((section(".boot_flag"))) volatile uint8_t boot_flag;
__attribute__((section(".boot_flag"))) volatile uint8_t boot_flag_1;
__attribute__((section(".boot_flag"))) volatile uint8_t boot_flag_2;
__attribute__((section(".boot_flag"))) volatile uint8_t boot_flag_3;

extern uint32_t __boot_code_start__; // NOLINT(*-reserved-identifier)
extern void     tasks_deinit(void);

void io_bootHandler_processBootRequest(const CanMsg *msg)
{
    if (msg->std_id == (BOARD_HIGHBITS | BOOT_CAN_START_LOWBITS))
    {
        boot_flag = 0x1;

        __disable_irq(); // disable interrupts
        __DSB();         // Complete all outstanding memory accesses
        __ISB();         // Synchronize the CPU pipeline

        //  Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
        for (uint32_t i = 0; i < 8; i++)
        {
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }
        boot_flag_1 = 0xa;
        boot_flag_2 = 0xb;
        boot_flag_3 = 0xc;
        NVIC_SystemReset();
    }
}
