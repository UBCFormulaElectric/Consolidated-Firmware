#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"

/**
 * @brief The main Chimera loop.
 * @param id_to_gpio A table of Protobuf-generated net names to Gpio pins.
 * @param id_to_adc A table of Protobuf-generated net names to ADC channels.
 * @param gpio_tag The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
 * @param adc_tag The Protobuf-generated tag for the board's ADC channels, defined in shared.pb.h.
 */
void io_chimera_v2_main(const Gpio *id_to_gpio[], const AdcChannel *id_to_adc[], pb_size_t gpio_tag, pb_size_t adc_tag);