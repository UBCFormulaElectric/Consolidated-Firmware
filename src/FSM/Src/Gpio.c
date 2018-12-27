/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Gpio.h"
#include "main.h"
/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
typedef enum
{
    BRAKE_PRESSED     = GPIO_PIN_SET,
    BRAKE_NOT_PRESSED = !BRAKE_PRESSED
} BspdBrakeThres_GPIO_PinState;

/** @brief The MAX3097E ALARM pin becomes high when there is an error */
typedef enum
{
    ALARM_ACTIVE_FAULT = GPIO_PIN_SET,
    ALARM_NO_FAULT     = !ALARM_ACTIVE_FAULT
} Max3097eAlarm_GPIO_PinState;

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Gpio_TurnOnRedLed(void)
{
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_SET);
}

void Gpio_TurnOnGreenLed(void)
{
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_SET);
}

void Gpio_TurnOnBlueLed(void)
{
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_SET);
}

bool Gpio_IsBrakePressed(void)
{
    return (
        HAL_GPIO_ReadPin(BSPD_BRAKE_THRES_GPIO_Port, BSPD_BRAKE_THRES_Pin) ==
        BRAKE_PRESSED);
}

bool Gpio_IsPappsAlarmdActive(void)
{
    return (
        HAL_GPIO_ReadPin(
            PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin) ==
        ALARM_ACTIVE_FAULT);
}

bool Gpio_IsSappsAlarmdActive(void)
{
    return (
        HAL_GPIO_ReadPin(
            SECONDARY_APPS_ALARM_GPIO_Port, SECONDARY_APPS_ALARM_Pin) ==
        ALARM_ACTIVE_FAULT);
}
