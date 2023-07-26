#include "avgPower.h"
#include <stdlib.h>
#include <assert.h>

typedef struct 
{
    bool     is_enabled;
    float    running_power; // current running value, will be a large sum
    uint16_t inc_counter;
    float    avg_power;
} AvgPowerData;

static AvgPowerData data;

void avgPower_init(void)
{
    avgPower_reset();
}

float avgPower_update(float instant_power)
{
    data.inc_counter++;
    data.running_power += instant_power;
    return data.running_power / data.inc_counter;
}

void avgPower_reset(void)
{
    data.is_enabled    = false;
    data.running_power = 0.0f;
    data.inc_counter   = 0U;
    data.avg_power     = 0.0f;
}
