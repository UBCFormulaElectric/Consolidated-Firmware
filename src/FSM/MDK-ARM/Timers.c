#include <stdint.h>
#include "stm32f0xx_hal.h"
#include "main.h"

extern TIM_HandleTypeDef htim14;
const intptr_t htim14ptr =  (intptr_t)&htim14;

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
			
     switch ((intptr_t)htim->Instance)
    {
        case (intptr_t)TIM14 :
            ControlLoop();
            break;
        default:
            break;
    }

}