#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    DACChannel_A,
    DACChannel_B,
    DACChannel_C,
    DACChannel_D,
    DACChannel_E,
    DACChannel_F,
    DACChannel_G,
    DACChannel_H,
    DACChannel_ALL,
} DACChannel;

bool io_dac_writeReg(DACChannel reg_addr, float volts);
bool io_dac_powerUp(DACChannel reg_addr);
bool io_dac_powerDown(DACChannel reg_addr);
void io_dac_clear();