#include "app_tractiveSystem.h"
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "io_tractiveSystem.h"

float app_tractiveSystem_getVoltage()
{
    return io_tractiveSystem_getVoltage();
}

float app_tractiveSystem_getCurrent(void)
{
    float low_res_current  = io_tractiveSystem_getCurrentLowResolution();
    float high_res_current = io_tractiveSystem_getCurrentHighResolution();

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

    App_CanTx_BMS_TractiveSystemVoltage_Set(ts_voltage);
    App_CanTx_BMS_TractiveSystemCurrent_Set(ts_current);
    App_CanTx_BMS_TractiveSystemPower_Set(ts_power);
}

bool app_tractveSystem_checkFaults()
{
    //    Charge current is positive, discharge current is negative
    //    TS current should be in the range: (-265.5, 70.8)
    const float current_A         = app_tractiveSystem_getCurrent();
    bool ts_current_out_of_bounds = !IS_IN_RANGE(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, current_A);
    App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Set(ts_current_out_of_bounds);

    return ts_current_out_of_bounds;
}
