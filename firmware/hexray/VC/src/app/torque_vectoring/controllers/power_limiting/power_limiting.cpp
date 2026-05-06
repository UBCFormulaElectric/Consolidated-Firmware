#include "power_limiting.hpp"
#include "motor_table.hpp"
#include <algorithm>
#include <cmath>

namespace CurrentLimiter
{

// Internal state

static Config s_cfg;
static float  s_last_alpha = 1.0f;
static float  s_alpha_trim = 1.0f; // Feedback trim, initialised to no correction

void init(const Config &cfg)
{
    s_cfg        = cfg;
    s_last_alpha = 1.0f;
    s_alpha_trim = 1.0f;
}

void update(const WheelInput inputs[N_WHEELS], WheelOutput outputs[N_WHEELS])
{
    // 1. Feed-forward - predict current draw per wheel

    float I_predicted[N_WHEELS];
    float I_total_predicted = 0.0f;

    for (int i = 0; i < N_WHEELS; i++)
    {
        if (s_cfg.use_dc_current)
        {
            I_predicted[i] = MotorTable::predict_dc_current_A(
                inputs[i].speed_rpm, inputs[i].torque_Nm, inputs[i].temp_C, inputs[i].dc_bus_voltage_V);
        }
        else
        {
            I_predicted[i] =
                MotorTable::predict_line_current_A(inputs[i].speed_rpm, inputs[i].torque_Nm, inputs[i].temp_C);
        }
        // Guard against negative prediction (regen operating points)
        I_predicted[i] = std::max(0.0f, I_predicted[i]);
        I_total_predicted += I_predicted[i];
    }

    // 2. Calculate feed-forward scaling factor alpha

    float alpha_ff = 1.0f;
    if (I_total_predicted > s_cfg.I_max_BMS_A)
    {
        alpha_ff = s_cfg.I_max_BMS_A / I_total_predicted;
    }

    // 3. Feedback trim: We do this to correct for steady-state prediction error which results in too much downscaling
    //    Use actual measured current sum vs BMS limit.

    float I_actual_total = 0.0f;
    for (int i = 0; i < N_WHEELS; i++)
    {
        I_actual_total += std::max(0.0f, inputs[i].current_meas_A);
    }

    float error = s_cfg.I_max_BMS_A - I_actual_total; // positive means headroom -> relax torquw limit, negative means
                                                      // we violated the current limit -> tighten torque limits
    s_alpha_trim = std::clamp(s_alpha_trim + s_cfg.Kp_feedback * error, 0.0f, 1.0f);

    // 4. Combined alpha — feed-forward carries most of the weight, trim corrects residuals and drift. Alpha should
    // never exceed 1.0 (don't increase torque beyond original request).

    float alpha_final = std::clamp(alpha_ff * s_alpha_trim, 0.0f, 1.0f);
    s_last_alpha      = alpha_final;

    // 5. Apply alpha to torque limits, pass speed commands unchanged.
    //    Yaw is preserved because all wheels scale by the same alpha,
    //    maintaining torque ratios that produce the target Mz.

    for (int i = 0; i < N_WHEELS; i++)
    {
        float T_limited = alpha_final * inputs[i].torque_Nm;

        outputs[i].speed_cmd_rpm       = inputs[i].speed_rpm;
        outputs[i].torque_limit_pos_Nm = T_limited;  // ID82
        outputs[i].torque_limit_neg_Nm = -T_limited; // ID83 (negative convention)
        outputs[i].predicted_current_A = I_predicted[i];
    }
}

float get_last_alpha()
{
    return s_last_alpha;
}

} // namespace CurrentLimiter
