#pragma once

#include <stdint.h>
#include "App_ErrorCode.h"

struct SocDigits
{
    uint8_t digits[3];
    uint8_t num_digits;
};

/**
 * Round the given state of charge to an integer value by dropping the decimal
 * values, and then convert it to individual digits.
 * @note Only the first num_digits values in digits[] are defined, the rest are
 *       undefined. For example, 67.857% state of charge will be converted to:
 *           digits[0] = 7
 *           digits[1] = 6
 *           digits[2] = (undefined because there are only 2 digits)
 *           num_digits = 2
 * @param soc The state of charge to convert
 * @param soc_digits The buffer for storing the individual digits
 * @return ERROR_CODE_INVALID_ARGS if the given state of charge is out-of-bound
 */
ErrorCode App_SocDigits_Convert(float soc, struct SocDigits *soc_digits);
