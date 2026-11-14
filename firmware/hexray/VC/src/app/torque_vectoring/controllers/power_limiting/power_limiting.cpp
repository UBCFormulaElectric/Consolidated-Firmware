// Feed-forward prediction + proportional feedback trim.
// Feed-forward enforces limit before dispatch.
// Feedback corrects steady-state prediction error from Kt drift and temperature.

#include "power_limiting.hpp"
#include "motor_table.hpp"
#include <algorithm>
#include <cmath>

namespace CurrentLimiter
{

// Internal state

static Config cfg;
static float  alpha_trim_discharge        = 1.0f;
static float  alpha_trim_charge           = 1.0f;
static float  last_alpha_discharge        = 1.0f;
static float  last_alpha_charge           = 1.0f;
static float  prev_torque_limit[N_WHEELS] = {};

void init(const Config &config)
{
    cfg                  = config;
    alpha_trim_discharge = 1.0f;
    alpha_trim_charge    = 1.0f;
    last_alpha_discharge = 1.0f;
    last_alpha_charge    = 1.0f;
    for (int i = 0; i < N_WHEELS; i++)
    {
        prev_torque_limit[i] = config.motor_max_torque;
    }
}

void update(const WheelInput inputs[N_WHEELS], WheelOutput outputs[N_WHEELS], float bms_dc_current)
{
    // 1. Predict current draw per wheel from previous torque limit.
    //    Wheel direction is determined by speed sign — negative speed means regen.
    //    regular and regen wheels are summed into separate buckets.

    float I_predicted[N_WHEELS];
    float I_total_discharge = 0.0f;
    float I_total_charge    = 0.0f;

    for (int i = 0; i < N_WHEELS; i++)
    {
        bool is_regen = inputs[i].speed_rpm < 0.0f;

        I_predicted[i] = MotorTable::predict_dc_current(
            inputs[i].speed_rpm, prev_torque_limit[i], inputs[i].temp_C, inputs[i].dc_bus_voltage);

        if (is_regen && cfg.regen_on)
        {
            I_total_charge += std::min(0.0f, I_predicted[i]);
        }
        else
        {
            I_total_discharge += std::max(0.0f, I_predicted[i]);
        }
    }

    // 2. Feedback trim
    //    error > 0 means headroom, error < 0 means violation.

    float discharge_error = cfg.I_max_BMS_discharge - bms_dc_current;
    float charge_error    = bms_dc_current - (-cfg.I_max_BMS_charge); // I_max_BMS_charge is assumed negative

    alpha_trim_discharge = std::clamp(alpha_trim_discharge + cfg.Kp_feedback * discharge_error, 0.0f, 1.0f);
    alpha_trim_charge    = std::clamp(alpha_trim_charge + cfg.Kp_feedback * charge_error, 0.0f, 1.0f);

    // 3. Compute per-wheel current budget proportional to predicted draw.
    //    Discharge and regen budgets are computed independently.

    float I_budget_discharge = cfg.I_max_BMS_discharge * alpha_trim_discharge;
    float I_budget_charge    = cfg.I_max_BMS_charge * alpha_trim_charge;

    float I_limit[N_WHEELS];
    for (int i = 0; i < N_WHEELS; i++)
    {
        bool is_regen = inputs[i].speed_rpm < 0.0f;
        if (is_regen && cfg.regen_on)
        {
            float fraction = (I_total_charge < 0.0f) ? I_predicted[i] / I_total_charge : 1.0f;
            I_limit[i]     = -I_budget_charge * fraction; // negative = charging
        }
        else
        {
            float fraction = (I_total_discharge > 0.0f) ? I_predicted[i] / I_total_discharge : 1.0f;
            I_limit[i]     = I_budget_discharge * fraction; // positive = discharging
        }
    }

    // 4. Convert current limits to torque limits via table lookup.
    //    Speed commands pass through unchanged.

    last_alpha_discharge =
        (I_total_discharge > 0.0f) ? std::clamp(I_budget_discharge / I_total_discharge, 0.0f, 1.0f) : 1.0f;
    last_alpha_charge = (I_total_charge < 0.0f) ? std::clamp(I_budget_charge / (-I_total_charge), 0.0f, 1.0f) : 1.0f;

    for (int i = 0; i < N_WHEELS; i++)
    {
        float T_limited = MotorTable::lookup_torque(inputs[i].speed_rpm, std::abs(I_limit[i]), inputs[i].temp_C);

        T_limited = std::max(0.0f, T_limited);

        prev_torque_limit[i]           = T_limited;
        outputs[i].speed_cmd_rpm       = inputs[i].speed_rpm;
        outputs[i].torque_limit_pos_Nm = T_limited;  // ID82
        outputs[i].torque_limit_neg_Nm = -T_limited; // ID83
        outputs[i].predicted_current   = I_predicted[i];
    }
}

float get_last_alpha_discharge()
{
    return last_alpha_discharge;
}
float get_last_alpha_charge()
{
    return last_alpha_charge;
}

} // namespace CurrentLimiter