#include "hw_adcs.hpp"
#include "main.h"
#include <cassert>

constexpr hw::adcchip<5> adc1{ hadc1, htim3 };
constexpr hw::Adc        lc3_out            = adc1.getChannel(0);
constexpr hw::Adc        susp_travel_rl_3v3 = adc1.getChannel(1);
constexpr hw::Adc        susp_travel_rr_3v3 = adc1.getChannel(2);
constexpr hw::Adc        bps_3v3            = adc1.getChannel(3);

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        assert(hadc == &adc1.gethadc());
        adc1.update_callback();
    }
}