#include "power_limiting.hpp"
#include "torque_vectoring/tv_debug.h"
#include "torque_vectoring/shared_datatypes/torque_limits.hpp"

namespace app::tv::controllers::power_limiting
{

using Wheel = app::tv::shared_datatypes::Wheel;
using namespace app::tv::shared_datatypes::torque_limits;

template <Decimal T>
shared_datatypes::wheel_set<T> limitPower(
    shared_datatypes::wheel_set<T> desired_motor_omegas,
    shared_datatypes::wheel_set<T> desired_motor_torques,
    shared_datatypes::wheel_set<T> torque_max,
    shared_datatypes::wheel_set<T> torque_min,
    T                              discharge_pl,
    T                              charge_pl)
{
    T sum_omegas         = 0;
    T sum_torques        = 0;
    T desired_power_draw = 0;

    for (const Wheel wheel : { Wheel::FL, Wheel::FR, Wheel::RL, Wheel::RR })
    {
        sum_omegas += desired_motor_omegas[wheel];
        sum_torques += desired_motor_torques[wheel];

        desired_power_draw += desired_motor_omegas[wheel] * desired_motor_torques[wheel];
    }

    T delta_torque;

    if (desired_power_draw < -1 * charge_pl)
    {
        delta_torque = (desired_power_draw + charge_pl) / sum_omegas;
    }
    else if (desired_power_draw > discharge_pl)
    {
        delta_torque = (desired_power_draw - discharge_pl) / sum_omegas;
    }
    else
    {
        delta_torque = 0;
    }

    const shared_datatypes::wheel_set<T> limited_torques = {
        .fl = std::clamp(desired_motor_torques.fl - delta_torque, torque_min.fl, torque_max.fl),
        .fr = std::clamp(desired_motor_torques.fr - delta_torque, torque_min.fr, torque_max.fr),
        .rl = std::clamp(desired_motor_torques.rl - delta_torque, torque_min.rl, torque_max.rl),
        .rr = std::clamp(desired_motor_torques.rr - delta_torque, torque_min.rr, torque_max.rr),
    };

    T limited_power_draw = 0;
    for (const Wheel wheel : { Wheel::FL, Wheel::FR, Wheel::RL, Wheel::RR })
    {
        limited_power_draw += desired_motor_omegas[wheel] * limited_torques[wheel];
    }

    tv_debug_data.plim = { .desired_power_draw = desired_power_draw,
                           .limited_power_draw = limited_power_draw,
                           .discharge_pl       = discharge_pl,
                           .charge_pl          = charge_pl };

    return limited_torques;
}

// don't forget to make an explicit instatiation request so the compiler compiles your function in the .lib file!
template shared_datatypes::wheel_set<float> limitPower<float>(
    shared_datatypes::wheel_set<float>,
    shared_datatypes::wheel_set<float>,
    shared_datatypes::wheel_set<float>,
    shared_datatypes::wheel_set<float>,
    float,
    float);

} // namespace app::tv::controllers::power_limiting