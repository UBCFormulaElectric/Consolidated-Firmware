#include "hw_adcs.h"
#include "main.h"

namespace hw::adc
{
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1 };
} // namespace hw::adc