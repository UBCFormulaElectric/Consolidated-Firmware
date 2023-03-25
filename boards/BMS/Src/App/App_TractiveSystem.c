#include "App_TractiveSystem.h"

struct TractiveSystem
{
    float (*get_ts_voltage)(void);
    float (*get_high_res_current)(void);
    float (*get_low_res_current)(void);
};

struct TractiveSystem *App_TractiveSystem_Create(
    float (*get_ts_voltage)(void),
    float (*get_high_res_current)(void),
    float (*get_low_res_current)(void))
{
    struct TractiveSystem *ts = malloc(sizeof(struct TractiveSystem));
    assert(ts != NULL);

    ts->get_ts_voltage       = get_ts_voltage;
    ts->get_high_res_current = get_high_res_current;
    ts->get_low_res_current  = get_low_res_current;

    return ts;
}

void App_TractiveSystem_Destroy(struct TractiveSystem *ts)
{
    free(ts);
}

float App_TractiveSystem_GetVoltage(struct TractiveSystem *ts)
{
    return ts->get_ts_voltage();
}

float App_TractiveSystem_GetHighResCurrent(struct TractiveSystem *ts)
{
    return ts->get_high_res_current();
}

float App_TractiveSystem_GetLowResCurrent(struct TractiveSystem *ts)
{
    return ts->get_low_res_current();
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

float App_TractiveSystem_GetPower(struct TractiveSystem *ts)
{
    return App_TractiveSystem_GetVoltage(ts) * App_TractiveSystem_GetLowResCurrent(ts);
}

bool App_TractveSystem_CheckFaults(struct TractiveSystem *ts)
{
    //    Charge current is negative, discharge current is positive
    //    TS current should be in the range: (-70.8,265.5)
    bool ts_current_out_of_bounds =
        !IS_IN_RANGE(MAX_TS_CHARGE_CURRENT_AMPS, MAX_TS_DISCHARGE_CURRENT_AMPS, App_TractiveSystem_GetCurrent(ts));

    App_CanTx_BMS_Faults_TSOverCurrentFault_Set(ts_current_out_of_bounds);

    return ts_current_out_of_bounds;
}
