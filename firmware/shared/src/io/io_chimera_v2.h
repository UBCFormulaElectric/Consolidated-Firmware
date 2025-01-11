#pragma once
#include <stdlib.h>

void io_chimera_main(
    uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len),
    uint32_t net_name_gpio,
    uint32_t net_name_adc);