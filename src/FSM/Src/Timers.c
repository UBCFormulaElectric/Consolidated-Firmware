/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Timers.h"
#include "Apps.h"

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
/**
 * @brief Main control loop
 */
static void ControlLoop(void);

/**
 * @brief Handle sensor data acquisition and transmission for accelerator pedal
 *        position, steering angle, and wheel speeds.
 */
static void HandleSensorData(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void ControlLoop(void)
{
    HandleSensorData();
}

static void HandleSensorData(void)
{
    Apps_HandleAcceleratorPedalPosition();
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM14)
    {
        ControlLoop();
    }
}

void Timers_StartTimers()
{
    // Start control loop timer
    HAL_TIM_Base_Start_IT(&htim14);

    // Start primary APPS encoder
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    // Start secondary APPS encoder
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
