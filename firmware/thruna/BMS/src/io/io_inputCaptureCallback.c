#include "hw_hal.h"
#include "io_imd.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim)
{
    io_imd_inputCaptureCallback(htim);
}
