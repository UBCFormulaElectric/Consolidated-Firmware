#include "app_math.h"

float app_math_linearDerating(const float x, const float max_y, const float roll_off_x, const float max_x)
{
    float derated_y = 0.0f;

    if (x <= roll_off_x)
    {
        derated_y = max_y;
    }
    else if (x > max_x)
    {
        derated_y = 0.0f;
    }
    else if (x > roll_off_x)
    {
        derated_y = max_y - max_y * (x - roll_off_x) / (max_x - roll_off_x);
    }

    return derated_y;
}
