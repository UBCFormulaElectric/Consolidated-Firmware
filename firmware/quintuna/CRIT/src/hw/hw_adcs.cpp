#include "hw_adcs.h"
#include "main.h"

namespace hw::adc
{
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1 };
const Adc                       regen_3v3_adc{ adc1.getChannel(ADC1_IN14_REGEN) };
} // namespace hw::adc

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    hw::adc::adc1.update_callback();
}