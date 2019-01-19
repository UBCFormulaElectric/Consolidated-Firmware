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
		
		WheelSpeedIncrementControlLoopCounter();
    HandleSensorData();
    
}

static void HandleSensorData(void)
{
    Apps_HandleAcceleratorPedalPosition();
	  WS_HandleWheelSpeed();
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &CONTROL_LOOP_TIMER)
    {
        ControlLoop();
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if (htim == &F_R_WHEELSPEED_TIMER){
       uint32_t timer_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        SetWheelSpeed(F_R_WHEEL, timer_val);
    }else if(htim == &F_L_WHEELSPEED_TIMER){
        uint32_t timer_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        SetWheelSpeed(F_L_WHEEL, timer_val);
    }
}

void Timers_StartTimers()
{
    HAL_TIM_Base_Start_IT(&CONTROL_LOOP_TIMER);

    HAL_TIM_Encoder_Start(&PAPPS_TIMER, TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_Start(&SAPPS_TIMER, TIM_CHANNEL_ALL);
	
	   //Start timers in inturrupt mode for Front Right/Left wheel speed sensors 
    //as input captures (Hall Effect)
    HAL_TIM_IC_Start_IT(&F_R_WHEELSPEED_TIMER, TIM_CHANNEL_1);
	   HAL_TIM_IC_Start_IT(&F_L_WHEELSPEED_TIMER, TIM_CHANNEL_1);	
}

