#include <stdlib.h>
#include <assert.h>
#include "App_AvgPower.h"
#include "App_CanAlerts.h"
#include "App_CanUtils.h"
#include "App_CanRx.h"

struct AvgPowerCalc
{
    bool     is_enabled;
    float    running_power; // current running value, will be a large sum
    uint16_t inc_counter;
    float    avg_power;
};

struct AvgPowerCalc *App_AvgPowerCalc_Create(void)
{
    struct AvgPowerCalc *avgPowerCalc = malloc(sizeof(struct AvgPowerCalc));
    assert(avgPowerCalc != NULL);

    avgPowerCalc->is_enabled    = false;
    avgPowerCalc->running_power = 0.0f;
    avgPowerCalc->inc_counter   = 0U;
    avgPowerCalc->avg_power     = 0.0f;

    return avgPowerCalc;
}

void App_AvgPowerCalc_Destroy(struct AvgPowerCalc *avgPowerCalc)
{
    free(avgPowerCalc);
}

float App_AvgPowerCalc_Update(struct AvgPowerCalc *avgPowerCalc, float instant_power)
{
    if (avgPowerCalc->is_enabled)
    {
        avgPowerCalc->inc_counter++;
        float instantaneous_power = instant_power;
        avgPowerCalc->running_power += instantaneous_power;
        avgPowerCalc->avg_power = avgPowerCalc->running_power / avgPowerCalc->inc_counter;
    }
    else
    {
        avgPowerCalc->avg_power = 0.0f;
    }

    return avgPowerCalc->avg_power;
}

void App_AvgPowerCalc_Reset(struct AvgPowerCalc *avgPowerCalc)
{
    avgPowerCalc->is_enabled    = false;
    avgPowerCalc->running_power = 0.0f;
    avgPowerCalc->inc_counter   = 0U;
    avgPowerCalc->avg_power     = 0.0f;
}

void App_AvgPowerCalc_Enable(struct AvgPowerCalc *avgPowerCalc, bool enabled)
{
    avgPowerCalc->is_enabled = enabled;
}
