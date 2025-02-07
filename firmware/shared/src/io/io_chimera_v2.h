#pragma once
#include <stdlib.h>
#include "hw_adc.h"
#include "hw_gpio.h"

// Run the main Chimera loop.
void io_chimera_v2_main(Gpio *id_to_gpio[], AdcChannel *id_to_adc[], uint32_t net_name_gpio, uint32_t net_name_adc);