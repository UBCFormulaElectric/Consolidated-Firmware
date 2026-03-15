#include <cstdint>
#include <cmath>

#include "io_charger.hpp"

namespace app::charger
{
class Charger
{
  public:
    static float getAvailableCurrent()
    {
        const float evseDutyCycle = io::charger::getCPDutyCycle();

        if (evseDutyCycle >= 0.1f && evseDutyCycle <= 0.85f)
        {
            return evseDutyCycle * 100.0f * 0.6f;
        }
        else if (evseDutyCycle > 0.85f && evseDutyCycle <= 0.97f)
        {
            return evseDutyCycle * 100.0f * 2.5f - 64.0f;
        }

        return 0.0f; // Sentinel value for invalid/critical conditions
    }
};
} // namespace app::charger