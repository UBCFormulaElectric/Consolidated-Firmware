#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "App_SocDigits.h"

struct SocDigits
{
    uint8_t digits[3];
    uint8_t num_digits;
};

struct SocDigits *App_SocDigits_Create(void)
{
    struct SocDigits *soc_digits = malloc(sizeof(struct SocDigits));

    assert(soc_digits != NULL);

    memset(soc_digits->digits, 0, sizeof(soc_digits->digits));
    soc_digits->num_digits = 0;

    return soc_digits;
}

void App_SocDigits_Destroy(struct SocDigits *const soc_digits)
{
    free(soc_digits);
}

ErrorCode App_SocDigits_Tick(struct SocDigits *const soc_digits, float soc)
{
    if (soc < 0.0f || soc > 100.0f)
    {
        return ERROR_CODE_INVALID_ARGS;
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

    return ERROR_CODE_OK;
}

const uint8_t *App_SocDigits_GetDigits(const struct SocDigits *const soc_digits)
{
    return soc_digits->digits;
}

uint8_t App_SocDigits_GetNumDigits(const struct SocDigits *const soc_digits)
{
    return soc_digits->num_digits;
}
