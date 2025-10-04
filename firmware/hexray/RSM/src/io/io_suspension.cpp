#include "io_suspension.hpp"
#include "hw_adcs.hpp" //Not yet created for hw
#include "hw_gpios.hpp"

#define METERS_PER_VOLT 100.0f
//QUESTION: From where??

static float travelFromVoltage(const float voltage)
{
    return voltage / METERS_PER_VOLT;
}

float io_suspension_getRLTravel()
{   
    //Could have diff suspension for diff address
    return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rl_3v3)); 
}

float io_suspension_getRRTravel()
{
    return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rr_3v3));
}

bool io_suspension_rr_OCSC()
{
    //Could have diff ocsc pin for diff address
    return hw_gpio_readPin(&susp_travel_rr_ocsc_pin);
}

bool io_suspension_rl_OCSC()
{
    return hw_gpio_readPin(&susp_travel_rl_ocsc_pin);
}