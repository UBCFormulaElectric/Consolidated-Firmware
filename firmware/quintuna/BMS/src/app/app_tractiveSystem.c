#include "app_tractiveSystem.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "io_tractiveSystem.h"
#include "app_timer.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)

// Taken from our cell's datasheet, https://www.molicel.com/wp-content/uploads/INR18650P26A-V2-80087.pdf
#define MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS (-35.0f)
#define MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS (6.0f)
#define STANDARD_TS_CHARGE_CURRENT_PER_CELL_AMPS (2.6f)

#define CELLS_IN_PARALLEL (5)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define MAX_TS_CHARGE_CURRENT_AMPS (MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define STANDARD_TS_CHARGE_CURRENT_AMPS (STANDARD_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)

#define TS_OVERCURRENT_DEBOUNCE_DURATION_MS (100U)

#define W_TO_KW 1.0e-3f

TimerChannel overcurrent_fault_timer;

void app_tractiveSystem_init(void)
{
    app_timer_init(&overcurrent_fault_timer, TS_OVERCURRENT_DEBOUNCE_DURATION_MS);
}

float app_tractiveSystem_getVoltage(void)
{
    return io_tractiveSystem_getVoltage();
}

bool app_tractiveSystem_getVoltageDiagState(void)
{
    return io_tractiveSystem_getVoltageDiagState();
}

bool app_tractiveSystem_getCurrentDiagState(void)
{
    return io_tractiveSystem_getCurrentDiagState();
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
    const float ts_voltage            = app_tractiveSystem_getVoltage();
    const float ts_current            = app_tractiveSystem_getCurrent();
    const float ts_power_kw           = ts_voltage * ts_current * W_TO_KW;
    const bool  ts_voltage_diag_state = app_tractiveSystem_getVoltageDiagState();
    const bool  ts_current_diag_state = app_tractiveSystem_getCurrentDiagState();

    app_canTx_BMS_TractiveSystemVoltage_set(ts_voltage);
    app_canTx_BMS_TractiveSystemCurrent_set(ts_current);
    app_canTx_BMS_TractiveSystemPower_set(ts_power_kw);
    app_canTx_BMS_VoltageDiagState_set(ts_voltage_diag_state);
    app_canTx_BMS_CurrentDiagState_set(ts_current_diag_state);

    //    Charge current is positive, discharge current is negative
    //    TS current should be in the range: (-175, 30)
    const float current_A = app_tractiveSystem_getCurrent();
    // Check if the current is out of range
    const bool ts_current_out_of_bounds =
        !IS_IN_RANGE(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, current_A);
    const bool ts_overcurrent_fault =
        app_timer_runIfCondition(&overcurrent_fault_timer, ts_current_out_of_bounds) == TIMER_STATE_EXPIRED;

    app_canAlerts_BMS_Warning_TractiveSystemOvercurrent_set(ts_overcurrent_fault);
}
