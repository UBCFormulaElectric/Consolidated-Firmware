#include <stm32f3xx_hal.h>

#include "Io_FlowMeters.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    Io_FlowMeters_InputCaptureCallback(htim);
}
