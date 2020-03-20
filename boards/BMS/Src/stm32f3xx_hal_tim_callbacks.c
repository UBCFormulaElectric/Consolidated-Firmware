#include <stm32f3xx_hal.h>

#include "Io/Io_Imd.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    Io_Imd_InputCaptureCallback(htim);
}
