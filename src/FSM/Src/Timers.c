/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
#include "Timers.h"
#include "WheelSpeed.h"

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
 * @brief Handle data acquisition and transmission for all sensors
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
    if (htim == &CONTROL_LOOP_TIMER)
    {
        ControlLoop();
    }else if (htim == &FRWHEELSPEED_TIMER){
        SetWheelSpeed(FR_WHEEL);
    }else if(htim == &FLWHEELSPEED_TIMER){
        SetWheelSpeed(FL_WHEEL);
    }
}

void Timers_StartTimers()
{
    HAL_TIM_Base_Start_IT(&CONTROL_LOOP_TIMER);

    HAL_TIM_Encoder_Start(&PAPPS_TIMER, TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_Start(&SAPPS_TIMER, TIM_CHANNEL_ALL);
}

