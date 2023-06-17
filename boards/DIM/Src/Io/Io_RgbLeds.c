#include <stm32f4xx.h>
#include "Io_RgbLeds.h"
#include "main.h"

void Io_RgbLeds_TurnDimStatusLedRed(void)
{
    //    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_SET);
    //    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnDimStatusLedGreen(void)
{
    //    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_SET);
    //    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnDimStatusLedBlue(void)
{
    //    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLeds_TurnOffDimStatusLed(void)
{
    //    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_RESET);
    //    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnDcmStatusLedRed(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnDcmStatusLedGreen(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnDcmStatusLedBlue(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLeds_TurnOffDcmStatusLed(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnFsmStatusLedRed(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnFsmStatusLedGreen(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnFsmStatusLedBlue(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLeds_TurnOffFsmStatusLed(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnBmsStatusLedRed(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnBmsStatusLedGreen(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnBmsStatusLedBlue(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLeds_TurnOffBmsStatusLed(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnPdmStatusLedRed(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnPdmStatusLedGreen(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_RgbLeds_TurnPdmStatusLedBlue(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_RgbLeds_TurnOffPdmStatusLed(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}
