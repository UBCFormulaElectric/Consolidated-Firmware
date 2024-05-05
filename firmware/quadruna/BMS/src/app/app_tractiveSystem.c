#include "app_tractiveSystem.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "io_tractiveSystem.h"

float app_tractiveSystem_getVoltage()
{
    return io_tractiveSystem_getVoltage();
}

float app_tractiveSystem_getCurrent(void)
{
    const float low_res_current  = io_tractiveSystem_getCurrentLowResolution();
    const float high_res_current = io_tractiveSystem_getCurrentHighResolution();

    // static float high_avg, low_avg = 0.0f;

    // high_avg = (0.1f * high_res_current + 0.9f * high_avg);
    // low_avg  = (0.1f * low_res_current + 0.9f * low_avg);

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
    const float ts_voltage = app_tractiveSystem_getVoltage();
    const float ts_current = app_tractiveSystem_getCurrent();
    const float ts_power   = ts_voltage * ts_current;

    app_canTx_BMS_TractiveSystemVoltage_set(ts_voltage);
    app_canTx_BMS_TractiveSystemCurrent_set(ts_current);
    app_canTx_BMS_TractiveSystemPower_set(ts_power);
}

bool app_tractveSystem_checkFaults()
{
    //    Charge current is positive, discharge current is negative
    //    TS current should be in the range: (-265.5, 70.8)
    const float current_A         = app_tractiveSystem_getCurrent();
    bool ts_current_out_of_bounds = !IS_IN_RANGE(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, current_A);
    app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_set(ts_current_out_of_bounds);

    return ts_current_out_of_bounds;
}
