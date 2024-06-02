#include "app_tractiveSystem.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "io_tractiveSystem.h"
#include "app_timer.h"

TimerChannel overcurrent_fault_timer;

void app_tractiveSystem_init()
{
    app_timer_init(&overcurrent_fault_timer, TS_OVERCURRENT_DEBOUNCE_DURATION_MS);
}

#define W_TO_KW 1.0e-3f

float app_tractiveSystem_getVoltage()
{
    return io_tractiveSystem_getVoltage();
}

float app_tractiveSystem_getCurrent(void)
{
    const float low_res_current  = io_tractiveSystem_getCurrentLowResolution();
    const float high_res_current = io_tractiveSystem_getCurrentHighResolution();

    if (IS_IN_RANGE(-HIGH_RES_MAX_CURRENT_READING, HIGH_RES_MAX_CURRENT_READING, low_res_current))
    {
        return high_res_current;
    }
    else
    {
        return low_res_current;
    }
}

void app_tractiveSystem_broadcast()
{
    const float ts_voltage  = app_tractiveSystem_getVoltage();
    const float ts_current  = app_tractiveSystem_getCurrent();
    const float ts_power_kw = ts_voltage * ts_current * W_TO_KW;

    app_canTx_BMS_TractiveSystemVoltage_set(ts_voltage);
    app_canTx_BMS_TractiveSystemCurrent_set(ts_current);
    app_canTx_BMS_TractiveSystemPower_set(ts_power_kw);
}

bool app_tractveSystem_checkFaults()
{
    //    Charge current is positive, discharge current is negative
    //    TS current should be in the range: (-265.5, 70.8)
    const float current_A = app_tractiveSystem_getCurrent();
    const bool  ts_current_out_of_bounds =
        !IS_IN_RANGE(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, current_A);
    const bool ts_overcurrent_fault =
        app_timer_runIfCondition(&overcurrent_fault_timer, ts_current_out_of_bounds) == TIMER_STATE_EXPIRED;

    app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_set(ts_overcurrent_fault);

    return ts_overcurrent_fault;
}
