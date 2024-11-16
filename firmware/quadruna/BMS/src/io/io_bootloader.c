#include "io_bootloader.h"
#include "io_bootloaders.h"
#include <stdint.h>
#include "hw_hal.h"

#define BOOT_CAN_START 1012

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;

extern ADC_HandleTypeDef   hadc1;
extern FDCAN_HandleTypeDef hfdcan1;
extern TIM_HandleTypeDef   htim1;
extern TIM_HandleTypeDef   htim3;
extern TIM_HandleTypeDef   htim15;
extern UART_HandleTypeDef  huart1;
extern SPI_HandleTypeDef   hspi2;
extern CRC_HandleTypeDef   hcrc;

void io_bootloader_checkBootMsg(CanMsg *msg)
{
    if (msg->std_id == BOOT_CAN_START)
    {
        HAL_TIM_Base_Start(&htim1);
        HAL_TIM_Base_Start(&htim3);
        HAL_TIM_Base_Start(&htim15);
        HAL_TIM_Base_DeInit(&htim1);
        HAL_TIM_Base_DeInit(&htim3);
        HAL_TIM_Base_DeInit(&htim15);

        HAL_UART_Abort_IT(&huart1);
        HAL_UART_DeInit(&huart1);

        HAL_ADC_Stop_IT(&hadc1);
        HAL_ADC_DeInit(&hadc1);

        HAL_SPI_Abort_IT(&hspi2);
        HAL_SPI_DeInit(&hspi2);

        HAL_CRC_DeInit(&hcrc);

        // acknowledge the msg sent by boot
        CanMsg reply = { .std_id = BOOT_CAN_START, .dlc = 0 };
        io_can_pushTxMsgToQueue(&reply);

        HAL_FDCAN_DeInit(&hfdcan1);

        modifyStackPointerAndStartApp(&__boot_code_start__);
    }
}