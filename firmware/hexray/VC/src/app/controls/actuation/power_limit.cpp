#include "controls/actuation/power_limit.hpp"
#include "controls/controls_debug.h"
#include "controls/torque_limits.hpp"

namespace app::controls::actuation::power_limit
{

using Wheel = app::controls::Wheel;
using namespace app::controls::torque_limits;

wheel_set<float> limitPower(
    wheel_set<float> motor_omega_radps,
    wheel_set<float> motor_torque_Nm,
    wheel_set<float> torque_max_Nm,
    wheel_set<float> torque_min_Nm,
    float            discharge_limit_W,
    float            charge_limit_W)
{
    float sum_motor_omega_radps = 0;
    float desired_power_W       = 0;

    for (const Wheel wheel : { Wheel::FL, Wheel::FR, Wheel::RL, Wheel::RR })
    {
        sum_motor_omega_radps += motor_omega_radps[wheel];
        desired_power_W += motor_omega_radps[wheel] * motor_torque_Nm[wheel];
    }

    float delta_torque_Nm;

    if (desired_power_W < -1 * charge_limit_W)
    {
        delta_torque_Nm = (desired_power_W + charge_limit_W) / sum_motor_omega_radps;
    }
    else if (desired_power_W > discharge_limit_W)
    {
        delta_torque_Nm = (desired_power_W - discharge_limit_W) / sum_motor_omega_radps;
    }
    else
    {
        delta_torque_Nm = 0;
    }

    const wheel_set<float> limited_torque_Nm = {
        .fl = std::clamp(motor_torque_Nm.fl - delta_torque_Nm, torque_min_Nm.fl, torque_max_Nm.fl),
        .fr = std::clamp(motor_torque_Nm.fr - delta_torque_Nm, torque_min_Nm.fr, torque_max_Nm.fr),
        .rl = std::clamp(motor_torque_Nm.rl - delta_torque_Nm, torque_min_Nm.rl, torque_max_Nm.rl),
        .rr = std::clamp(motor_torque_Nm.rr - delta_torque_Nm, torque_min_Nm.rr, torque_max_Nm.rr),
    };

    float limited_power_W = 0;
    for (const Wheel wheel : { Wheel::FL, Wheel::FR, Wheel::RL, Wheel::RR })
    {
        limited_power_W += motor_omega_radps[wheel] * limited_torque_Nm[wheel];
    }

    tv_debug_data.plim = { .desired_power_W   = desired_power_W,
                           .limited_power_W   = limited_power_W,
                           .discharge_limit_W = discharge_limit_W,
                           .charge_limit_W    = charge_limit_W };

    return limited_torque_Nm;
}

// don't forget to make an explicit instatiation request so the compiler compiles your function in the .lib file!

} // namespace app::controls::actuation::power_limit