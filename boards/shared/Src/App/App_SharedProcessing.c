#include "App_SharedProcessing.h"

float App_SharedProcessing_LinearDerating(float x, float max_y, float roll_off_x, float max_x)
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
