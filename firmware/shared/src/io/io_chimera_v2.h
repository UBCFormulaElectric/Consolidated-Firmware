#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"

// Run the main Chimera loop.
void io_chimera_v2_main(const Gpio *id_to_gpio[], const AdcChannel *id_to_adc[], pb_size_t gpio_tag, pb_size_t adc_tag);