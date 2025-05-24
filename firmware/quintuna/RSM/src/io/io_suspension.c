#include "io_suspension.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

#define METERS_PER_VOLT (100.0f)

static float travelFromVoltage(float voltage)
{
    float travel_m = (voltage / METERS_PER_VOLT);
    return travel_m;
}

float io_suspension_getRLTravel(void)
{
    float voltage = hw_adc_getVoltage(&susp_travel_rl_3v3);
    return travelFromVoltage(voltage);
}

float io_suspension_getRRTravel(void)
{
    float voltage = hw_adc_getVoltage(&susp_travel_rr_3v3);
    return travelFromVoltage(voltage);
}

bool io_suspension_rr_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rr_ocsc_pin);
}

bool io_suspension_rl_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rl_ocsc_pin);
}
