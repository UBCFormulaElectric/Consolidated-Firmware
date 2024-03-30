#include "app_pumpControl.h"
#include "io_i2cPotentiometer.h"

const float SCALE_FACTOR = 15.625f; // 2000/128

void app_pumpControl_setFlowRate(float rate)
{
    if (io_i2cPotentiometer_Init())
    {
        io_i2cPotentiometer_setWiper((uint8_t) (rate / SCALE_FACTOR));
    }
}

float app_pumpControl_getFlowRate()
{
    return SCALE_FACTOR * (float) io_i2cPotentiometer_readWiper();
}