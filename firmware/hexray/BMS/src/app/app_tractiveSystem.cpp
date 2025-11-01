#include "app_tractiveSystem.hpp"
#include "app_timer.hpp"
#include "io_tractiveSystem.hpp"
#include "app_utils.hpp"

#define HIGH_RES_MAX_CURRENT_READING 50.0f
#define W_TO_KW 1.0e-3f

using namespace app;
namespace app::ts
{
static Timer overcurrent_warning_timer;

void init()
{
    overcurrent_warning_timer = Timer(TS_OVERCURRENT_DEBOUNCE_DURATION_MS);
}

float getVoltage()
{
    return io::ts::getVoltage();
}

float getCurrent()
{
    const float low_res_current  = io::ts::getLowResCurrent();
    const float high_res_current = io::ts::getHighResCurrent();
    if (IS_IN_RANGE(-HIGH_RES_MAX_CURRENT_READING, HIGH_RES_MAX_CURRENT_READING, low_res_current))
    {
        return high_res_current;
    }
    else
    {
        return low_res_current;
    }
}

void broadcast()
{
    const float ts_voltage  = getVoltage();
    const float ts_current  = getCurrent();
    const float ts_power_kw = ts_voltage * ts_current * W_TO_KW;

    const bool ts_voltage_sns_diag_state = io_tractiveSystem_getVoltageSnsDiagState();
    const bool ts_current_sns_diag_state = io_tractiveSystem_getCurrentSnsDiagState();
    const bool ts_overcurrent_warning =
        overcurrent_warning_timer.runIfCondition(!IS_IN_RANGE(
            MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, ts_current)) == Timer::TimerState::RUNNING;

    app_canTx_BMS_TractiveSystemVoltage_set(ts_voltage);
    app_canTx_BMS_TractiveSystemCurrent_set(ts_current);
    app_canTx_BMS_TractiveSystemPower_set(ts_power_kw);
    app_canTx_BMS_VoltageSensorOk_set(ts_voltage_sns_diag_state);
    app_canTx_BMS_CurrentSensorOk_set(ts_current_sns_diag_state);
    app_canTx_BMS_Warning_TsOvercurrent_set(ts_overcurrent_warning);
}

} // namespace app::ts
