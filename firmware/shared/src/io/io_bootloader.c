#include <stdint.h>

#include "io_bootloader.h"

#include "cmsis_gcc.h"
#include "cmsis_os.h"

#include "hw_hal.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "hw_flash.h"
#include "io_log.h"

#define BOOT_CAN_START 1012

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;
extern uint32_t __app_metadata_start__;

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

void io_bootloader_checkBootMsg(CanMsg *msg)
{
    if (msg->std_id == BOOT_CAN_START)
    {
        // DeInit all peripherals and interupts
        HAL_ADC_Stop_IT(&hadc1);
        HAL_ADC_Stop_IT(&hadc3);
        HAL_ADC_DeInit(&hadc1);
        HAL_ADC_DeInit(&hadc3);
        HAL_TIM_Base_Stop_IT(&htim3);
        HAL_UART_Abort_IT(&huart1);
        HAL_UART_Abort_IT(&huart2);
        HAL_UART_Abort_IT(&huart3);
        HAL_UART_Abort_IT(&huart7);
        HAL_UART_DeInit(&huart1);
        HAL_UART_DeInit(&huart2);
        HAL_UART_DeInit(&huart3);
        HAL_UART_DeInit(&huart7);

        boot_flag = 0x01;

        // acknowledge the msg sent by boot
        CanMsg reply = { .std_id = BOOT_CAN_START, .dlc = 0 };
        io_can_pushTxMsgToQueue(&reply);

        __disable_irq();

        // Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
        for (uint32_t i = 0; i < 8; i++)
        {
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }

        NVIC_SystemReset();
    }
}
