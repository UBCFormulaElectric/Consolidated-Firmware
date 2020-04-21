#include "App_SocDigits.h"

ExitCode App_SocDigits_Convert(float soc, struct SocDigits *const soc_digits)
{
    if (soc < 0.0f || soc > 100.0f)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    // The decimal points must be truncated from state of charge because the
    // 7-segment display isn't responsible for showing any decimal values
    uint32_t integer_soc = soc;

    // Turn the state of charge into individual digits. We treat 0% state of
    // charge as having 1 digit, which is why num_soc_digits starts counting
    // from 1.
    for (uint8_t num_soc_digits = 1; num_soc_digits <= 3; num_soc_digits++)
    {
        soc_digits->digits[num_soc_digits - 1] = integer_soc % 10;
        integer_soc /= 10;

        if (integer_soc == 0)
        {
            soc_digits->num_digits = num_soc_digits;
            break;
        }
    }

    return EXIT_CODE_OK;
}
