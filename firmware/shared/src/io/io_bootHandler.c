#include <stdint.h>

#include "io_bootHandler.h"

#include "cmsis_gcc.h"
#include "cmsis_os.h"

#include "hw_hal.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "hw_flash.h"
#include "io_log.h"

#define BOOT_CAN_START 1012

extern uint32_t __boot_code_start__;

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
extern TIM_HandleTypeDef   htim3;
extern UART_HandleTypeDef  huart2;
extern UART_HandleTypeDef  huart1;
extern UART_HandleTypeDef  huart3;
extern SD_HandleTypeDef    hsd1;

__attribute__((section(".boot_flag"))) uint8_t boot_flag;

void io_bootHandler_processBootRequest(CanMsg *msg, void (*deinit_func)(void))
{
    if (msg->std_id == BOOT_CAN_START)
    {
        boot_flag = 0x1;

        if (deinit_func != NULL)
        {
            deinit_func();
        }

        // acknowledge the msg sent by boot
        CanMsg reply = { .std_id = BOOT_CAN_START, .dlc = 0 };
        io_can_pushTxMsgToQueue(&reply);

        __disable_irq();
        __DSB(); // Complete all outstanding memory accesses
        __ISB(); // Synchronize the CPU pipeline

        //  Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
        for (uint32_t i = 0; i < 8; i++)
        {
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }

        NVIC_SystemReset();
    }
}
