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

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
	
/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void gpioTurnOnRedLed(void)
{
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_SET);
}

void gpioTurnOnGreenLed(void)
{
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_SET);
}

void gpioTurnOnBlueLed(void)
{
    HAL_GPIO_WritePin(STATUS_B_GPIO_Port, STATUS_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STATUS_R_GPIO_Port, STATUS_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STATUS_G_GPIO_Port, STATUS_G_Pin, GPIO_PIN_SET);
}