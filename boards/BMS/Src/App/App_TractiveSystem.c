#include <stdlib.h>
#include <assert.h>

struct TractiveSystem
{
    float (*get_ts_voltage)(float);
};

struct TractiveSystem *App_TractiveSystem_Create(float (*get_ts_voltage)(float))
{
    struct TractiveSystem *ts = malloc(sizeof(struct TractiveSystem));
    assert(ts != NULL);

    ts->get_ts_voltage     = get_ts_voltage;

    return ts;
}

void App_TractiveSystem_Destroy(struct TractiveSystem *ts)
{
    free(ts);
}

//TODO: TS_VOLTAGE_TUTORIAL: write App function to retrieve the value returned by the IO function.
//                          Reference App_TractiveSystem.h for the function prototype
