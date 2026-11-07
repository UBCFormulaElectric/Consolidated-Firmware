#include "speed_controller.hpp"
#include "torque_vectoring/controllers/controllers_config.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include <algorithm>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::speed_controller
{
static PID pid_fl(PID_SPEED_config);
static PID pid_fr(PID_SPEED_config);
static PID pid_rl(PID_SPEED_config);
static PID pid_rr(PID_SPEED_config);

static float wheelTorque(
    PID        &pid,
    const float speed_request_radps,
    const float omega_radps,
    const float tire_ffb_Nm,
    const float torque_max_Nm,
    const float torque_min_Nm)
{
    // The tire feedforward goes in as the PID's disturbance input, and the result is limited to this wheel's torque
    // limits.
    return std::clamp(pid.compute(speed_request_radps, omega_radps, tire_ffb_Nm), torque_min_Nm, torque_max_Nm);
}

static float calculateTireForceFeedback(const float F_x_N, const float radial_deflection_m, const float M_y_Nm)
{
    const float loaded_radius_m = std::fmax(WHEEL_RADIUS_M - radial_deflection_m, 0.0f);
    const float wheel_torque_Nm = M_y_Nm - F_x_N * loaded_radius_m;
    return -wheel_torque_Nm / GEAR_RATIO;
}

template <Decimal T>
shared_datatypes::wheel_set<T> computeTorque(
    const shared_datatypes::wheel_set<T> &speed_request_radps,
    const shared_datatypes::wheel_set<T> &omega_radps,
    const shared_datatypes::wheel_set<T> &F_x_N,
    const shared_datatypes::wheel_set<T> &radial_deflection_m,
    const shared_datatypes::wheel_set<T> &M_y_Nm,
    const shared_datatypes::wheel_set<T> &torque_max_Nm,
    const shared_datatypes::wheel_set<T> &torque_min_Nm)
{
    const shared_datatypes::wheel_set<T> tire_ffb_Nm = {
        .fl = calculateTireForceFeedback(F_x_N.fl, radial_deflection_m.fl, M_y_Nm.fl),
        .fr = calculateTireForceFeedback(F_x_N.fr, radial_deflection_m.fr, M_y_Nm.fr),
        .rl = calculateTireForceFeedback(F_x_N.rl, radial_deflection_m.rl, M_y_Nm.rl),
        .rr = calculateTireForceFeedback(F_x_N.rr, radial_deflection_m.rr, M_y_Nm.rr),
    };

    return { .fl = wheelTorque(
                 pid_fl, speed_request_radps.fl, omega_radps.fl, tire_ffb_Nm.fl, torque_max_Nm.fl, torque_min_Nm.fl),
             .fr = wheelTorque(
                 pid_fr, speed_request_radps.fr, omega_radps.fr, tire_ffb_Nm.fr, torque_max_Nm.fr, torque_min_Nm.fr),
             .rl = wheelTorque(
                 pid_rl, speed_request_radps.rl, omega_radps.rl, tire_ffb_Nm.rl, torque_max_Nm.rl, torque_min_Nm.rl),
             .rr = wheelTorque(
                 pid_rr, speed_request_radps.rr, omega_radps.rr, tire_ffb_Nm.rr, torque_max_Nm.rr, torque_min_Nm.rr) };
}
template shared_datatypes::wheel_set<tv_real> computeTorque(
    const shared_datatypes::wheel_set<tv_real> &speed_request_radps,
    const shared_datatypes::wheel_set<tv_real> &omega_radps,
    const shared_datatypes::wheel_set<tv_real> &F_x_N,
    const shared_datatypes::wheel_set<tv_real> &radial_deflection_m,
    const shared_datatypes::wheel_set<tv_real> &M_y_Nm,
    const shared_datatypes::wheel_set<tv_real> &torque_max_Nm,
    const shared_datatypes::wheel_set<tv_real> &torque_min_Nm);

void reset()
{
    pid_fl.reset();
    pid_fr.reset();
    pid_rl.reset();
    pid_rr.reset();
}

} // namespace app::tv::controllers::speed_controller
