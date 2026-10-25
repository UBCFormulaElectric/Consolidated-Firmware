#include "app_tractiveSystem.hpp"
#include "app_timer.hpp"
#include "app_math.hpp"
#include "io_tractiveSystem.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

constexpr float HIGH_RES_MAX_CURRENT_READING = 70.0f;       // Maximum current reading for high-resolution sensor (75 A, but 70 to be safe)
constexpr float W_TO_KW                      = 1.0e-3f;     // Conversion factor from Watts to kiloWatts

namespace app::ts 
{
// Initialize timer to check whether there is an overcurrent
static Timer overcurrent_warning_timer{TS_OVERCURRENT_DEBOUNCE_DURATION_MS};

float getVoltage()
{
    return io::ts::getVoltage();
}

float getCurrent()
{
    const float low_res_current   = io::ts::getCurrentLowResolution();
    const float high_res_current  = io::ts::getCurrentHighResolution();

    // Use low res measurement to determine whether the high res sensor is within 
    // its valid ±70 A range. If it is, we use the high res measurement instead
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
    const float ts_power_kw = ts_voltage * ts_current * W_TO_KW;    // P = v * i, convert to kW

    // Check sensor diagnostics; reads pin to determine if they are working properly
    const bool ts_voltage_sns_diag_state = io::ts::getVoltageSnsDiagState();
    const bool ts_current_sns_diag_state = io::ts::getCurrentSnsDiagState();

    // Check if current is within the safe range
    const bool current_out_of_range = !IS_IN_RANGE(
        MAX_TS_DISCHARGE_CURRENT_AMPS, 
        MAX_TS_CHARGE_CURRENT_AMPS, 
        ts_current);

    // Pass the current state boolean to the timer, and returns the current state of the timer
    const Timer::TimerState overcurrent_timer_state = 
        overcurrent_warning_timer.runIfCondition(current_out_of_range);

    const bool ts_overcurrent_warning = 
        overcurrent_timer_state == Timer::TimerState::RUNNING;    // might need fix to check for expired state instead?  


    // Broadcast all diagnostics over CAN 
    app::can_tx::BMS_TractiveSystemVoltage_set(ts_voltage);
    app::can_tx::BMS_TractiveSystemCurrent_set(ts_current);
    app::can_tx::BMS_TractiveSystemPower_set(ts_power_kw);
    app::can_tx::BMS_VoltageSensorOk_set(ts_voltage_sns_diag_state);
    app::can_tx::BMS_CurrentSensorOk_set(ts_current_sns_diag_state);
    app::can_alerts::warnings::TsOvercurrent_set(ts_overcurrent_warning);

}
} // namespace app::ts
