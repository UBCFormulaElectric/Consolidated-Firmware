#include "main.h"
#include "Io_RgbLedSequence.h"

void Io_RgbLedSequence_TurnOnRedLed(void)
{
    HAL_GPIO_WritePin(INV_RED_GPIO_Port, INV_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(INV_GREEN_GPIO_Port, INV_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INV_BLUE_GPIO_Port, INV_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLedSequence_TurnOnGreenLed(void)
{
    HAL_GPIO_WritePin(INV_RED_GPIO_Port, INV_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INV_GREEN_GPIO_Port, INV_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(INV_BLUE_GPIO_Port, INV_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLedSequence_TurnOnBlueLed(void)
{
    HAL_GPIO_WritePin(INV_RED_GPIO_Port, INV_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INV_GREEN_GPIO_Port, INV_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INV_BLUE_GPIO_Port, INV_BLUE_Pin, GPIO_PIN_RESET);
}
