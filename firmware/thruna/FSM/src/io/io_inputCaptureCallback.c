#include "hw_hal.h"

#include "io_coolant.h"
#include "io_wheels.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    io_flowMeter_inputCaptureCallback(htim);
    io_wheelSpeedSensors_inputCaptureCallback(htim);
}
