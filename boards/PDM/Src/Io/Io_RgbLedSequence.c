#include "main.h"
#include "Io_RgbLedSequence.h"

void Io_RgbLedSequence_TurnOnRedLed(void)
{
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_RESET);
}

void Io_RgbLedSequence_TurnOnGreenLed(void)
{
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_RESET);
}

void Io_RgbLedSequence_TurnOnBlueLed(void)
{
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_RESET);
}
