// #include <stdlib.h>
#include "app_avgPower.h"
#include <assert.h>

typedef struct
{
    bool     is_enabled;
    float    running_power; // current running value, will be a large sum
    uint16_t inc_counter;
    float    avg_power;
} AvgPowerCalc;

static AvgPowerCalc calc;

void app_avgPower_init(void)
{
    app_avgPower_reset();
}

float app_avgPower_update(float instant_power)
{
    if (calc.is_enabled)
    {
        calc.inc_counter++;
        float instantaneous_power = instant_power;
        calc.running_power += instantaneous_power;
        calc.avg_power = calc.running_power / calc.inc_counter;
    }
    else
    {
        calc.avg_power = 0.0f;
    }

    return calc.avg_power;
}

void app_avgPower_reset()
{
    calc.is_enabled    = false;
    calc.running_power = 0.0f;
    calc.inc_counter   = 0U;
    calc.avg_power     = 0.0f;
}

void app_avgPower_enable(bool enabled)
{
    calc.is_enabled = enabled;
}
