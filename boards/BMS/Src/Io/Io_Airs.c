#include "Io_Airs.h"
#include "main.h"

// Since there are no pins to determine the AIRs' statuses directly, the AIRs'
// statuses can be determined by the LUT below:
//
// +------------------+------------------+------------------------+
// | AIR POWER STATUS | AIR POWER ISENSE |      AIRs statuses     |
// +------------------+------------------+------------------------+
// |                0 |                0 | AIR- OPEN, AIR+ OPEN   |
// |                0 |                1 | NOT POSSIBLE           |
// |                1 |                0 | AIR- CLOSED, AIR+ OPEN |
// |                1 |                1 | AIR- CLOSED, AIR+ OPEN |
// +------------------+------------------+------------------------+

bool Io_Airs_IsAirNegativeClosed(void)
{
    return HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);
}

bool Io_Airs_IsAirPositiveClosed(void)
{
    return HAL_GPIO_ReadPin(AIR_TOTAL_ISENSE_GPIO_Port, AIR_TOTAL_ISENSE_Pin) &&
           HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);
}

void Io_Airs_CloseAirPositive(void)
{
    HAL_GPIO_WritePin(PRE_CHARGE_EN_GPIO_Port, PRE_CHARGE_EN_Pin, GPIO_PIN_SET);
}

void Io_Airs_OpenAirPositive(void)
{
    HAL_GPIO_WritePin(
        PRE_CHARGE_EN_GPIO_Port, PRE_CHARGE_EN_Pin, GPIO_PIN_RESET);
}
