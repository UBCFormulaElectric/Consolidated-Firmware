#include "app_pumpControl.h"
#include "io_i2cPotentiometer.h"
#include "app_canAlerts.h"

#define FLOW_RATE_LOWER_LIMIT (0.0f)
#define FLOW_RATE_UPPER_LIMIT (2000.0f)

const float SCALE_FACTOR = 15.625f; // 2000/128

void app_pumpControl_setFlowRate(float rate_lpm)
{
    bool flow_rate_out_of_range = false;

    if (rate_lpm < FLOW_RATE_LOWER_LIMIT)
    {
        rate_lpm               = FLOW_RATE_LOWER_LIMIT;
        flow_rate_out_of_range = true;
    }
    else if (rate_lpm > FLOW_RATE_UPPER_LIMIT)
    {
        rate_lpm               = FLOW_RATE_UPPER_LIMIT;
        flow_rate_out_of_range = true;
    }

    app_canAlerts_VC_Warning_FlowRateInputOutOfRange_set(flow_rate_out_of_range);

    if (io_i2cPotentiometer_Init())
    {
        io_i2cPotentiometer_setWiper((uint8_t)(rate_lpm / SCALE_FACTOR));
    }
}

float app_pumpControl_getFlowRate(void)
{
    return SCALE_FACTOR * (float)io_i2cPotentiometer_readWiper();
}
