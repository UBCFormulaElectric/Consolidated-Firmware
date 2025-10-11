#include "bootloader.h"
#include "main.h"
#include "stm32h7xx_hal_gpio.h"

void bootloader_boardSpecific_init(void)
{
    // Disable all loads other than the boards
    HAL_GPIO_WritePin(RL_PUMP_EN_GPIO_Port, RL_PUMP_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RR_PUMP_EN_GPIO_Port, RR_PUMP_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R_INV_EN_GPIO_Port, R_INV_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(F_INV_EN_GPIO_Port, F_INV_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FRONT_EN_GPIO_Port, FRONT_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(L_RAD_FAN_EN_GPIO_Port, L_RAD_FAN_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R_RAD_FAN_EN_GPIO_Port, R_RAD_FAN_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_EN_GPIO_Port, BMS_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RSM_EN_GPIO_Port, RSM_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DAM_EN_GPIO_Port, DAM_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FRONT_EN_GPIO_Port, FRONT_EN_Pin, GPIO_PIN_SET);
}