#pragma once

#include <stdint.h>
#include "App_ErrorCode.h"

struct SocDigits
{
    uint8_t digits[3];
    uint8_t num_digits;
};

struct SocDigits *App_SocDigits_Create(void);
void              App_SocDigits_Destroy(struct SocDigits *soc_digits);
ErrorCode         App_SocDigits_Tick(struct SocDigits *soc_digits, float soc);
const uint8_t *   App_SocDigits_GetDigits(const struct SocDigits *soc_digits);
uint8_t App_SocDigits_GetNumDigits(const struct SocDigits *soc_digits);
