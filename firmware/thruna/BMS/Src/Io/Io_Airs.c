#include "Io_Adc.h"
#include "Io_Airs.h"
#include "main.h"

// TODO: Remove static var and use these #defines to check if AIR positive has closed after HW issues are resolved
static bool is_air_positive_closed = false;

// Shunt resistor used to measure AIR loop current
#define AIR_TOTAL_ISENSE_SHUNT_R_OHMS (0.075f)

// Amplifier used to amplify voltage drop across the shunt resistor
#define AIR_TOTAL_ISENSE_AMPLIFIER (20.0f)

// Converts voltage measured across shunt resistor to current in A
#define AIR_TOTAL_ISENSE_V_TO_A (1.0f / (AIR_TOTAL_ISENSE_SHUNT_R_OHMS * AIR_TOTAL_ISENSE_AMPLIFIER))

// AIR positive closed threshold
#define AIR_POSITIVE_CLOSED_THRESH_A (0.5f)

bool Io_Airs_IsAirNegativeClosed(void)
{
    return HAL_GPIO_ReadPin(AIR_PWR_STATUS_GPIO_Port, AIR_PWR_STATUS_Pin);
}

bool Io_Airs_IsAirPositiveClosed(void)
{
    return is_air_positive_closed;
}

void Io_Airs_CloseAirPositive(void)
{
    HAL_GPIO_WritePin(AIR_EN_GPIO_Port, AIR_EN_Pin, GPIO_PIN_SET);
    is_air_positive_closed = true;
}

void Io_Airs_OpenAirPositive(void)
{
    HAL_GPIO_WritePin(AIR_EN_GPIO_Port, AIR_EN_Pin, GPIO_PIN_RESET);
    is_air_positive_closed = false;
}
