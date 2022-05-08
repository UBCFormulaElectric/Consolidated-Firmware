#include <stdlib.h>
#include <assert.h>

struct TractiveSystem
{
    float (*get_raw_ts_voltage)(void);
    float (*get_ts_voltage)(float);
};

struct TractiveSystem *App_TractiveSystem_Create(float (*get_ts_raw_voltage)(void), float (*get_ts_voltage)(float))
{
    struct TractiveSystem *ts = malloc(sizeof(struct TractiveSystem));
    assert(ts != NULL);

    ts->get_raw_ts_voltage = get_ts_raw_voltage;
    ts->get_ts_voltage     = get_ts_voltage;

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
