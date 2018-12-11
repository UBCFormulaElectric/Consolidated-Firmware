#include "Timers.h"


const intptr_t htim14ptr =  (intptr_t)&htim14;

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim TIM handle
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(htim==&htim14){
		ControlLoop();
	}

}


void Timers_StartTimers(){
	/* Start main control loop timer, with interrupt.*/
	HAL_TIM_Base_Start_IT(&htim14);
	
	/* Start Main APPS Encoder*/
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);  
	/* Start Secondary APPS encoder. */
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);  
	
}