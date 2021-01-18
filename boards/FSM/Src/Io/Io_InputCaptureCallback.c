#include <stm32f3xx_hal.h>

#include "Io_YS201.h"
#include "Io_WheelSpeedSensors.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    Io_YS201_InputCaptureCallback(htim);
    Io_WheelSpeedSensors_InputCaptureCallback(htim);
}
