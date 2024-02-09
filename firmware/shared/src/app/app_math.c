#include "app_math.h"

float app_math_linearDerating(float x, float max_y, float roll_off_x, float limit_x, bool derating_direction)
{
    float derated_y = 0.0f;

    if(derating_direction)
    {
         if (x <= roll_off_x)
        {
            derated_y = max_y;
        }
        else if (x > limit_x)
        {
            derated_y = 0.0f;
        }
        else if (x > roll_off_x)
        {
            derated_y = max_y - max_y * (x - roll_off_x) / (limit_x - roll_off_x);
        }
    }
    else
    {
        if (x >= roll_off_x)
        {
            derated_y = max_y;
        }
        else if (x <= limit_x)
        {
            derated_y = 0.0f;
        }
        else if (x < roll_off_x)
        {
            derated_y = max_y - max_y * (roll_off_x - x) / (roll_off_x - limit_x);
        }
        }
    

    return derated_y;
}
