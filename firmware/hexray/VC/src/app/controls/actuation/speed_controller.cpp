#include "controls/actuation/speed_controller.hpp"
#include "controls/params.hpp"
#include "controls/controls_debug.h"
#include <algorithm>

using namespace app::controls::params;

namespace app::controls::actuation::speed_controller
{
static constexpr app::PID::Config PID_SPEED_config{
    .Kp               = 0.2f,
    .Ki               = 4.0f,
    .Kd               = 0.0f,
    .Kb               = 0.0f,
    .Kff              = 1.0f,
    .smoothing_coeff  = 1.0f,
    .out_max          = 21.0f,
    .out_min          = -15.0f,
    .max_integral     = 21.0f / 4.0f,
    .min_integral     = -15.0f / 4.0f,
    .clamp_output     = true,
    .clamp_integral   = true,
    .back_calculation = false,
    .feed_forward     = true,
    .sample_time      = 0.01f // 10 ms sample time (100 Hz)
};

static PID pid_fl(PID_SPEED_config);
static PID pid_fr(PID_SPEED_config);
static PID pid_rl(PID_SPEED_config);
static PID pid_rr(PID_SPEED_config);

static float motorTorque(
    PID        &pid,
    const float motor_speed_request_radps,
    const float motor_omega_radps,
    const float tire_ffb_Nm,
    const float torque_max_Nm,
    const float torque_min_Nm)
{
    // The tire feedforward goes in as the PID's disturbance input, and the result is limited to this wheel's torque
    // limits.
    return std::clamp(
        pid.compute(motor_speed_request_radps, motor_omega_radps, tire_ffb_Nm), torque_min_Nm, torque_max_Nm);
}

static float calculateTireForceFeedback(const float fx_N, const float radial_deflection_m, const float my_Nm)
{
    const float loaded_radius_m = std::fmax(WHEEL_RADIUS_M - radial_deflection_m, 0.0f);
    const float wheel_torque_Nm = my_Nm - fx_N * loaded_radius_m;
    return -wheel_torque_Nm / GEAR_RATIO;
}

template <Decimal T>
wheel_set<T> computeTorque(
    const wheel_set<T> &motor_speed_request_radps,
    const wheel_set<T> &motor_omega_radps,
    const wheel_set<T> &fx_N,
    const wheel_set<T> &radial_deflection_m,
    const wheel_set<T> &my_Nm,
    const wheel_set<T> &torque_max_Nm,
    const wheel_set<T> &torque_min_Nm)
{
    const wheel_set<T> tire_ffb_Nm = {
        .fl = calculateTireForceFeedback(fx_N.fl, radial_deflection_m.fl, my_Nm.fl),
        .fr = calculateTireForceFeedback(fx_N.fr, radial_deflection_m.fr, my_Nm.fr),
        .rl = calculateTireForceFeedback(fx_N.rl, radial_deflection_m.rl, my_Nm.rl),
        .rr = calculateTireForceFeedback(fx_N.rr, radial_deflection_m.rr, my_Nm.rr),
    };

    const wheel_set<T> torque_Nm = {
        .fl = motorTorque(
            pid_fl, motor_speed_request_radps.fl, motor_omega_radps.fl, tire_ffb_Nm.fl, torque_max_Nm.fl,
            torque_min_Nm.fl),
        .fr = motorTorque(
            pid_fr, motor_speed_request_radps.fr, motor_omega_radps.fr, tire_ffb_Nm.fr, torque_max_Nm.fr,
            torque_min_Nm.fr),
        .rl = motorTorque(
            pid_rl, motor_speed_request_radps.rl, motor_omega_radps.rl, tire_ffb_Nm.rl, torque_max_Nm.rl,
            torque_min_Nm.rl),
        .rr = motorTorque(
            pid_rr, motor_speed_request_radps.rr, motor_omega_radps.rr, tire_ffb_Nm.rr, torque_max_Nm.rr,
            torque_min_Nm.rr),
    };

    motor_speed_request_radps.copyTo(tv_debug_data.speed_ctrl.motor_speed_request_radps);
    tire_ffb_Nm.copyTo(tv_debug_data.speed_ctrl.tire_ffb_Nm);
    torque_Nm.copyTo(tv_debug_data.speed_ctrl.desired_torque_Nm);

    return torque_Nm;
}
template wheel_set<tv_real> computeTorque(
    const wheel_set<tv_real> &motor_speed_request_radps,
    const wheel_set<tv_real> &motor_omega_radps,
    const wheel_set<tv_real> &fx_N,
    const wheel_set<tv_real> &radial_deflection_m,
    const wheel_set<tv_real> &my_Nm,
    const wheel_set<tv_real> &torque_max_Nm,
    const wheel_set<tv_real> &torque_min_Nm);

void reset()
{
    pid_fl.reset();
    pid_fr.reset();
    pid_rl.reset();
    pid_rr.reset();
}

} // namespace app::controls::actuation::speed_controller
