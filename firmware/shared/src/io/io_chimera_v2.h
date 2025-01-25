#pragma once
#include <stdlib.h>
#include "hw_adc.h"
#include "hw_gpio.h"

void io_chimera_v2_main(
    Gpio       *id_to_gpio[],
    AdcChannel *id_to_adc[],
    uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len),
    uint32_t net_name_gpio,
    uint32_t net_name_adc);