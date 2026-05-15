#include <cstdint>
#include <cmath>

#include "io_charger.hpp"

namespace app::charger
{
float getAvailableCurrent()
{
    // It is not directly related to this module, but we do not expect to ever exceed 16A from the
    // AC circuit at competition due to breaker limits + cell constraints + charger limits.
    // As a result, we do not expect to exceed ~35% duty cycle on evse under any circumstance.
    const float evseDutyCycle = io::charger::getCPDutyCycle();

    if (evseDutyCycle >= 10.0f && evseDutyCycle <= 85.0f)
    {
        return evseDutyCycle * 0.6f;
    }
    else if (evseDutyCycle > 85.0f && evseDutyCycle <= 97.0f)
    {
        return evseDutyCycle * 2.5f - 64.0f;
    }

    return 0.0f; // Sentinel value for invalid/critical conditions
}
} // namespace app::charger