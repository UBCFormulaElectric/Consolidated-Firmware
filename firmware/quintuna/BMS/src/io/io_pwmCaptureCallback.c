#include "hw_hal.h"
#include "io_imd.h"
#include "main.h"
#include <assert.h>

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim1){
        io_imd_inputCaptureCallback(htim);
    }
    else if(htim == &htim5){
        // io_evse_inputCaptureCallback(htim)
    }
    else{
        assert(false);
    }
}
