#include "io_bootloader.h"
#include "io_bootloaders.h"
#include <stdint.h>
#include "hw_hal.h"

#define BOOT_CAN_START 1012

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;

extern ADC_HandleTypeDef   hadc1;
extern CAN_HandleTypeDef   hcan1;
extern TIM_HandleTypeDef   htim3;
extern UART_HandleTypeDef  huart2;

void io_bootloader_checkBootMsg(CanMsg *msg)
{
    if (msg->std_id == BOOT_CAN_START)
    {
        // DeInit all peripherals and interrupts
        HAL_TIM_Base_Stop_IT(&htim3);
        HAL_TIM_Base_DeInit(&htim3);

        HAL_UART_Abort_IT(&huart2);
        HAL_UART_DeInit(&huart2);

        HAL_ADC_Stop_IT(&hadc1);
        HAL_ADC_DeInit(&hadc1);

        // acknowledge the msg sent by boot
        CanMsg reply = { .std_id = BOOT_CAN_START, .dlc = 0 };
        io_can_pushTxMsgToQueue(&reply);

        HAL_CAN_DeInit(&hcan1);

        io_bootloader_modifyStackPointer(&__boot_code_start__);
    }
}