#include "io_suspension.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

#define M (0.0f)
#define B (0.0f) // GET M AND B FROM RSM DATA

const Suspension right = { .src = &susp_travel_rr_3v3 };
const Suspension left  = { .src = &susp_travel_rl_3v3 };

float travelFromVoltage(float voltage) 
{
    float travel_mm = (voltage - B) / M;
    return travel_mm;
}

float io_suspension_getrlTravel(void)
{
    float voltage = hw_adc_getVoltage(&susp_travel_rl_3v3);
    return travelFromVoltage(voltage);
}

float io_suspension_getrrTravel(void)
{
    float voltage = hw_adc_getVoltage(&susp_travel_rr_3v3);
    return travelFromVoltage(voltage);
}
\
bool io_suspension_rr_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rr_ocsc_pin);
}

bool io_suspension_rl_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rl_ocsc_pin);
}
