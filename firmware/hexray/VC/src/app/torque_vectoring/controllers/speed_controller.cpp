#include "speed_controller.hpp"
#include "torque_vectoring/controllers/controllers_config.hpp"
#include <algorithm>

namespace app::tv::controllers::speed_controller
{
static PID pid_fl(PID_SPEED_config);
static PID pid_fr(PID_SPEED_config);
static PID pid_rl(PID_SPEED_config);
static PID pid_rr(PID_SPEED_config);

static float wheelTorque(
    PID        &pid,
    const float request,
    const float omega,
    const float tire_ffb,
    const float torque_max,
    const float torque_min)
{
    pid.setOutputLimits(torque_min, torque_max); // Update output limits based on torque limits
    return pid.compute(request, omega, tire_ffb);
}

shared_datatypes::wheel_set<float> computeTorque(
    const shared_datatypes::wheel_set<float> &speed_request_radps,
    const shared_datatypes::wheel_set<float> &omega_radps,
    const shared_datatypes::wheel_set<float> &tire_ffb_Nm,
    const shared_datatypes::wheel_set<float> &torque_max_Nm,
    const shared_datatypes::wheel_set<float> &torque_min_Nm)
{
    return { .fl = wheelTorque(
                 pid_fl, speed_request_radps.fl, omega_radps.fl, tire_ffb_Nm.fl, torque_max_Nm.fl, torque_min_Nm.fl),
             .fr = wheelTorque(
                 pid_fr, speed_request_radps.fr, omega_radps.fr, tire_ffb_Nm.fr, torque_max_Nm.fr, torque_min_Nm.fr),
             .rl = wheelTorque(
                 pid_rl, speed_request_radps.rl, omega_radps.rl, tire_ffb_Nm.rl, torque_max_Nm.rl, torque_min_Nm.rl),
             .rr = wheelTorque(
                 pid_rr, speed_request_radps.rr, omega_radps.rr, tire_ffb_Nm.rr, torque_max_Nm.rr, torque_min_Nm.rr) };
}

void reset()
{
    pid_fl.reset();
    pid_fr.reset();
    pid_rl.reset();
    pid_rr.reset();
}

} // namespace app::tv::controllers::speed_controller
