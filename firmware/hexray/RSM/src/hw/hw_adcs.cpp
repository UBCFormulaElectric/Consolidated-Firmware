#include "hw_adcs.hpp"
#include "main.h"
#include <cassert>

constexpr hw::AdcChip<5> Adc_Chip{ hadc1, htim3 };
constexpr hw::Adc        lc3_out            = Adc_Chip.getChannel(0);
constexpr hw::Adc        susp_travel_rl_3v3 = Adc_Chip.getChannel(1);
constexpr hw::Adc        susp_travel_rr_3v3 = Adc_Chip.getChannel(2);
constexpr hw::Adc        bps_3v3            = Adc_Chip.getChannel(3);

void chipsInit()
{
    LOG_IF_ERR(Adc_Chip.init());
}

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        assert(hadc == &Adc_Chip.gethadc());
        Adc_Chip.update_callback();
    }
}