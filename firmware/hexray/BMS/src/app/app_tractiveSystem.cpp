#include "app_tractiveSystem.hpp"
#include "app_timer.hpp"
#include "app_math.hpp"
#include "io_tractiveSystem.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

inline constexpr float HIGH_RES_MAX_CURRENT_READING = 50.0f;
inline constexpr float W_TO_KW                      = 1.0e-3f;

namespace app::ts
{
static Timer overcurrent_warning_timer{ TS_OVERCURRENT_DEBOUNCE_DURATION_MS };

void init() {}

float getVoltage() {
    // TS voltage settling/noise is handled in hardware via a long ADC sampling
    // time on the vsense channels (810.5 cycles, configured in CubeMX). No
    // software filter is applied.
    return io::ts::getVoltage();
}

float getCurrent()
{
    const float low_res_current  = io::ts::getCurrentLowResolution();
    const float high_res_current = io::ts::getCurrentHighResolution();
    if (IS_IN_RANGE(-HIGH_RES_MAX_CURRENT_READING, HIGH_RES_MAX_CURRENT_READING, low_res_current))
    {
        return high_res_current;
    }
    return low_res_current;
}

void broadcast()
{
    const float ts_voltage  = getVoltage();
    const float ts_current  = getCurrent();
    const float ts_power_kw = ts_voltage * ts_current * W_TO_KW;

    const bool ts_voltage_sns_diag_state = io::ts::getVoltageSnsDiagState();
    const bool ts_current_sns_diag_state = io::ts::getCurrentSnsDiagState();
    const bool ts_overcurrent_warning =
        overcurrent_warning_timer.runIfCondition(!IS_IN_RANGE(
            MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_TS_CHARGE_CURRENT_AMPS, ts_current)) == Timer::TimerState::RUNNING;

    app::can_tx::BMS_TractiveSystemVoltage_set(ts_voltage);
    app::can_tx::BMS_TractiveSystemCurrent_set(ts_current);
    app::can_tx::BMS_TractiveSystemPower_set(ts_power_kw);
    app::can_tx::BMS_VoltageSensorOk_set(ts_voltage_sns_diag_state);
    app::can_tx::BMS_CurrentSensorOk_set(ts_current_sns_diag_state);
    app::can_alerts::warnings::TsOvercurrent_set(ts_overcurrent_warning);
}
} // namespace app::ts
