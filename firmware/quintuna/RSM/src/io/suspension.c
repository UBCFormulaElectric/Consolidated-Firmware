#include "io/suspension.h"
#include "hw/adcs.h"
#include "hw/gpios.h"

static float travelFromVoltage(const float voltage)
{
#define METERS_PER_VOLT (100.0f)
    return voltage / METERS_PER_VOLT;
}

float io_suspension_getRLTravel(void)
{
    return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rl_3v3));
}

float io_suspension_getRRTravel(void)
{
    return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rr_3v3));
}

bool io_suspension_rr_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rr_ocsc_pin);
}

bool io_suspension_rl_OCSC(void)
{
    return hw_gpio_readPin(&susp_travel_rl_ocsc_pin);
}
