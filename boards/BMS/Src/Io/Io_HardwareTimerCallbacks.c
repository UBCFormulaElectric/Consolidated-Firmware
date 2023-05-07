#include "Io_Hal.h"
#include "Io_Imd.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  Io_Imd_InputCaptureCallback(htim);
}
