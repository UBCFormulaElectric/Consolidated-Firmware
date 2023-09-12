#include "main.h"
#include "Io_Hal.h"
#include "Io_LatchedFaults.h"

bool Io_LatchedFaults_CheckImdLatchedFault(void)
{
    return HAL_GPIO_ReadPin(IMD_LATCH_GPIO_Port, IMD_LATCH_Pin) == GPIO_PIN_RESET;
}

bool Io_LatchedFaults_CheckBspdLatchedFault(void)
{
    return HAL_GPIO_ReadPin(BSPD_LATCH_GPIO_Port, BSPD_LATCH_Pin) == GPIO_PIN_RESET;
}

bool Io_LatchedFaults_CheckBmsLatchedFault(void)
{
    return HAL_GPIO_ReadPin(BMS_LATCH_GPIO_Port, BMS_LATCH_Pin) == GPIO_PIN_RESET;
}
