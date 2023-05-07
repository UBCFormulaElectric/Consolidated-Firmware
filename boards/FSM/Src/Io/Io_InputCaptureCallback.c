#include "Io_Coolant.h"
#include "Io_Hal.h"
#include "Io_WheelSpeedSensors.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  Io_FlowMeter_InputCaptureCallback(htim);
  Io_WheelSpeedSensors_InputCaptureCallback(htim);
}
