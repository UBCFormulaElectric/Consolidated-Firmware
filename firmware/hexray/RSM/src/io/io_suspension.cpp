#include "io_suspension.hpp"
// #include "hw_adcs.hpp" //Not yet created for hw
// #include "hw_gpios.hpp"

#define METERS_PER_VOLT 100.0f

static float travelFromVoltage(const float voltage)
{
    return voltage / METERS_PER_VOLT;
}

namespace io::suspension
{
    float getRLTravel()
    {
        //Could have diff suspension for diff address
        // return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rl_3v3)); 
    }

    float getRRTravel()
    {
        // return travelFromVoltage(hw_adc_getVoltage(&susp_travel_rr_3v3));
    }

    bool RR_OCSC()
    {
        //Could have diff ocsc pin for diff address
        // return hw_gpio_readPin(&susp_travel_rr_ocsc_pin);
    }

    bool RL_OCSC()
    {
        // return hw_gpio_readPin(&susp_travel_rl_ocsc_pin);
    }
}
