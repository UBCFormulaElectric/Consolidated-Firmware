#pragma once

#include <stdint.h>
#include "App_ErrorCode.h"

struct SocDigits
{
    uint8_t digits[3];
    uint8_t num_digits;
};

ErrorCode App_SocDigits_Convert(float soc, struct SocDigits *soc_digits);
