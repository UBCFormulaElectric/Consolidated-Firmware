#include "app_pumpControl.h"
#include "io_i2cPotentiometer.h"
#include <assert.h>

const float SCALE_FACTOR = 15.625f; // 2000/128

void app_pumpControl_setFlowRate(float rate_lpm)
{
    assert(rate_lpm >= 0.0f && rate_lpm <= 2000.0f);
    if (io_i2cPotentiometer_Init())
    {
        io_i2cPotentiometer_setWiper((uint8_t)(rate_lpm / SCALE_FACTOR));
    }
}

float app_pumpControl_getFlowRate(void)
{
    return SCALE_FACTOR * (float)io_i2cPotentiometer_readWiper();
}
