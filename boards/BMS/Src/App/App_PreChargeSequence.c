#include <assert.h>
#include <stdlib.h>
#include "App_PreChargeSequence.h"

struct PreCharge
{
    void (*enable)(void);
    void (*disable)(void);
    float (*get_raw_ts_voltage)(void);
    float (*get_ts_voltage)(float);
    uint32_t min_duration_ms;
    uint32_t min_complete_duration_ms;
    uint32_t max_complete_duration_ms;
};

struct PreCharge *App_PreCharge_Create(
    void (*enable)(void),
    void (*disable)(void),
    float (*get_raw_ts_voltage)(void),
    float (*get_ts_voltage)(float),
    uint32_t min_duration_ms,
    uint32_t min_complete_duration_ms,
    uint32_t max_complete_duration_ms)
{
    struct PreCharge *pre_charge = malloc(sizeof(struct PreCharge));
    assert(pre_charge != NULL);

    pre_charge->enable                   = enable;
    pre_charge->disable                  = disable;
    pre_charge->get_raw_ts_voltage       = get_raw_ts_voltage;
    pre_charge->get_ts_voltage           = get_ts_voltage;
    pre_charge->min_duration_ms          = min_duration_ms;
    pre_charge->min_complete_duration_ms = min_complete_duration_ms;
    pre_charge->max_complete_duration_ms = max_complete_duration_ms;

    return pre_charge;
}

void App_PreCharge_Destroy(struct PreCharge *pre_charge)
{
    free(pre_charge);
}

void App_PreCharge_Enable(const struct PreCharge *const pre_charge)
{
    pre_charge->enable();
}

void App_PreCharge_Disable(const struct PreCharge *const pre_charge)
{
    pre_charge->disable();
}

uint32_t App_PreCharge_GetTimeoutMs(const struct PreCharge *const pre_charge)
{
    return pre_charge->min_duration_ms;
}

uint32_t App_PreCharge_GetMinCompleteDurationMs(
    const struct PreCharge *const pre_charge)
{
    return pre_charge->min_complete_duration_ms;
}

uint32_t App_PreCharge_GetMaxCompleteDurationMs(
    const struct PreCharge *const pre_charge)
{
    return pre_charge->max_complete_duration_ms;
}
