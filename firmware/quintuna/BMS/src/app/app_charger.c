#include "app_charger.h"
#include "io_charger.h"

float app_charger_getAvaliableCurrent()
{
    const float evse_duty_cycle = io_charger_getCPDutyCycle();

    if (evse_duty_cycle >= 0.1f && evse_duty_cycle <= 0.85f)
    {
        return evse_duty_cycle * 100.0f * 0.6f;
    }

    if (evse_duty_cycle > 0.85f && evse_duty_cycle <= 0.97f)
    {
        return evse_duty_cycle * 100.0f * 2.5f - 64.0f;
    }

    return 0; // zero is catastrophic enough for it to be a sentinel value
}