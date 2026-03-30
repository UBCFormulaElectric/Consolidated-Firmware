#include "app_math.hpp"

namespace app::math
{

float linearDerating(float x, float max_y, float roll_off_x, float limit_x, bool derating_direction)
{
    float derated_y = 0.0f;

    // Logic for REDUCE_X (derating as x increases)
    if (derating_direction)
    {
        if (x <= roll_off_x)
        {
            derated_y = max_y;
        }
        else if (x > limit_x)
        {
            derated_y = 0.0f;
        }
        else // x is between roll_off_x and limit_x
        {
            // Calculate linear drop
            derated_y = max_y - max_y * (x - roll_off_x) / (limit_x - roll_off_x);
        }
    }
    // Logic for INCREASE_X (derating as x decreases)
    else
    {
        derated_y = 0.0f;

        if (x >= roll_off_x)
        {
            derated_y = max_y;
        }
        else if (x <= limit_x)
        {
            derated_y = 0.0f;
        }
        else // x is between limit_x and roll_off_x
        {
            // Calculate linear drop
            derated_y = max_y - max_y * (roll_off_x - x) / (roll_off_x - limit_x);
        }
    }

    return derated_y;
}

} // namespace app::math