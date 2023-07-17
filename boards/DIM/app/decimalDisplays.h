#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    DECIMAL_DISPLAY_BANK_L,
    DECIMAL_DISPLAY_BANK_M,
    DECIMAL_DISPLAY_BANK_R,
} DecimalDisplayBank;

void decimalDisplays_init(void);
bool decimalDisplays_displayNumber(DecimalDisplayBank bank, float value);
