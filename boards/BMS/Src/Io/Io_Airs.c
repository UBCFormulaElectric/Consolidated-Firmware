#include "Io_Airs.h"
#include "main.h"

// Since there are no pins to determine the AIRs' statuses directly, the AIRs'
// statuses can be determined by the LUT below:
//
// +------------------+------------------+--------------------+
// | AIR POWER STATUS | AIR POWER ISENSE |   AIRs statuses    |
// +------------------+------------------+--------------------+
// |                0 |                0 | AIR- OFF, AIR+ OFF |
// |                0 |                1 | NOT POSSIBLE       |
// |                1 |                0 | AIR- ON, AIR+ OFF  |
// |                1 |                1 | AIR- ON, AIR+ ON   |
// +------------------+------------------+--------------------+

bool Io_Airs_IsAirPositiveOn(void)
{
    return HAL_GPIO_ReadPin(AIR_TOTAL_ISENSE_GPIO_Port, AIR_TOTAL_ISENSE_Pin) &&
           HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);
}

bool Io_Airs_IsAirNegativeOn(void)
{
    return HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);
}
