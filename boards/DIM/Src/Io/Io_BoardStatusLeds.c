#include <stm32f3xx.h>
#include "Io_BoardStatusLeds.h"
#include "main.h"

void Io_BoardStatusLeds_TurnOnDimRed(void)
{
    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnDimGreen(void)
{
    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnDimBlue(void)
{
    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIM_GREEN_GPIO_Port, DIM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIM_BLUE_GPIO_Port, DIM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_BoardStatusLeds_TurnOnDcmRed(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnDcmGreen(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnDcmBlue(void)
{
    HAL_GPIO_WritePin(DCM_RED_GPIO_Port, DCM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_GREEN_GPIO_Port, DCM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCM_BLUE_GPIO_Port, DCM_BLUE_Pin, GPIO_PIN_SET);
}

void Io_BoardStatusLeds_TurnOnFsmRed(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnFsmGreen(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnFsmBlue(void)
{
    HAL_GPIO_WritePin(FSM_RED_GPIO_Port, FSM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FSM_GREEN_GPIO_Port, FSM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FSM_BLUE_GPIO_Port, FSM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnBmsRed(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnBmsGreen(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnBmsBlue(void)
{
    HAL_GPIO_WritePin(BMS_RED_GPIO_Port, BMS_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BMS_GREEN_GPIO_Port, BMS_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_BLUE_GPIO_Port, BMS_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnPdmRed(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnPdmGreen(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}

void Io_BoardStatusLeds_TurnOnPdmBlue(void)
{
    HAL_GPIO_WritePin(PDM_RED_GPIO_Port, PDM_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PDM_GREEN_GPIO_Port, PDM_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PDM_BLUE_GPIO_Port, PDM_BLUE_Pin, GPIO_PIN_RESET);
}
