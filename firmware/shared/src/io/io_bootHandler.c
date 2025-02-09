#include <stdint.h>

// cmis:
#include "cmsis_gcc.h"
#include "cmsis_os.h"

// hw:
#include "hw_hal.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "hw_flash.h"

// io:
#include "io_bootHandler.h"
#include "io_log.h"

#define BOOT_CAN_START 1012

__attribute__((section(".boot_flag"))) uint8_t boot_flag;

extern uint32_t __boot_code_start__;
extern void     tasks_deinit(void);

void io_bootHandler_processBootRequest(const CanMsg *msg)
{
    if (msg->std_id == BOOT_CAN_START)
    {
        boot_flag = 0x1;
        tasks_deinit();

        __disable_irq(); // disable interrupts
        __DSB();         // Complete all outstanding memory accesses
        __ISB();         // Synchronize the CPU pipeline

        //  Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
        for (uint32_t i = 0; i < 8; i++)
        {
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }
        NVIC_SystemReset();
    }
}
