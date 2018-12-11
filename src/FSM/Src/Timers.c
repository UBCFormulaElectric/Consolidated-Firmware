#include "Timers.h"


const intptr_t htim14ptr =  (intptr_t)&htim14;

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim TIM handle
  * @return None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(htim==&htim14){
		ControlLoop();
	}

}

/**
  * @brief  Initializes timers; using the appropriate HAL functions, params, and settings.
  * @param  None
  * @return None
  */
void Timers_StartTimers(){
	/* Start main control loop timer, with interrupt.*/
	HAL_TIM_Base_Start_IT(&htim14);
	
	/* Start Main APPS Encoder*/
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);  
	/* Start Secondary APPS encoder. */
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);  
	
}