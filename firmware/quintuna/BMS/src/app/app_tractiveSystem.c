#include "app_tractiveSystem.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "io_tractiveSystem.h"
#include "app_timer.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)
#define W_TO_KW 1.0e-3f

TimerChannel overcurrent_warning_timer;

void app_tractiveSystem_init(void)
{
    app_timer_init(&overcurrent_warning_timer, TS_OVERCURRENT_DEBOUNCE_DURATION_MS);
}

float app_tractiveSystem_getVoltage(void)
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

void app_tractiveSystem_broadcast(void)
{
    const float ts_voltage                = app_tractiveSystem_getVoltage();
    const float ts_current                = app_tractiveSystem_getCurrent();
    const float ts_power_kw               = ts_voltage * ts_current * W_TO_KW;
    const bool  ts_voltage_sns_diag_state = io_tractiveSystem_getVoltageSnsDiagState();
    const bool  ts_current_sns_diag_state = io_tractiveSystem_getCurrentSnsDiagState();
    const bool  ts_overcurrent_warning =
        app_timer_runIfCondition(
            &overcurrent_warning_timer,
            !IS_IN_RANGE(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, ts_current)) == TIMER_STATE_RUNNING;

    app_canTx_BMS_TractiveSystemVoltage_set(ts_voltage);
    app_canTx_BMS_TractiveSystemCurrent_set(ts_current);
    app_canTx_BMS_TractiveSystemPower_set(ts_power_kw);
    app_canTx_BMS_VoltageSensorOk_set(ts_voltage_sns_diag_state);
    app_canTx_BMS_CurrentSensorOk_set(ts_current_sns_diag_state);
    app_canTx_BMS_Warning_TsOvercurrent_set(ts_overcurrent_warning);
}
