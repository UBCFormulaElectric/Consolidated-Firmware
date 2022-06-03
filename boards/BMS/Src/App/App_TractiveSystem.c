#include <stdlib.h>
#include <assert.h>

#define HIGH_RES_MAX_CURRENT_READING (50.0f)

struct TractiveSystem
{
    float (*get_raw_ts_voltage)(void);
    float (*get_ts_voltage)(float);
    float (*get_adc_high_res_current)(void);
    float (*get_high_res_current)(float);
    float (*get_adc_low_res_current)(void);
    float (*get_low_res_current)(float);
};

struct TractiveSystem *App_TractiveSystem_Create(
    float (*get_ts_raw_voltage)(void),
    float (*get_ts_voltage)(float),
    float (*get_adc_high_res_current)(void),
    float (*get_high_res_current)(float),
    float (*get_adc_low_res_current)(void),
    float (*get_low_res_current)(float))
{
    struct TractiveSystem *ts = malloc(sizeof(struct TractiveSystem));
    assert(ts != NULL);

    ts->get_raw_ts_voltage       = get_ts_raw_voltage;
    ts->get_ts_voltage           = get_ts_voltage;
    ts->get_adc_high_res_current = get_adc_high_res_current;
    ts->get_high_res_current     = get_high_res_current;
    ts->get_adc_low_res_current  = get_adc_low_res_current;
    ts->get_low_res_current      = get_low_res_current;

    return ts;
}

void App_TractiveSystem_Destroy(struct TractiveSystem *ts)
{
    free(ts);
}

float App_TractiveSystem_GetVoltage(struct TractiveSystem *ts)
{
    return ts->get_ts_voltage(ts->get_raw_ts_voltage());
}

float App_TractiveSystem_GetHighResCurrent(struct TractiveSystem *ts)
{
    return ts->get_high_res_current(ts->get_adc_high_res_current());
}

float App_TractiveSystem_GetLowResCurrent(struct TractiveSystem *ts)
{
    return ts->get_low_res_current(ts->get_adc_low_res_current());
}

float App_TractiveSystem_GetCurrent(struct TractiveSystem *ts)
{
    float low_res_current  = App_TractiveSystem_GetLowResCurrent(ts);
    float high_res_current = App_TractiveSystem_GetHighResCurrent(ts);

    if (low_res_current < HIGH_RES_MAX_CURRENT_READING)
    {
        return high_res_current;
    }
    else
    {
        return low_res_current;
    }
}
