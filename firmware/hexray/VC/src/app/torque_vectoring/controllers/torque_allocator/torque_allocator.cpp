#include "torque_allocator.hpp"

#include <algorithm>
#include <cmath>

using namespace app::tv::datatypes::vd_constants;
using WheelSide = app::tv::estimation::TireModel::WheelSide;
using WheelAxle = app::tv::estimation::TireModel::WheelAxle;

namespace app::tv::controllers::allocator
{
namespace
{

// ---- Optimizer tuning (module-private) ----

static constexpr float W_FX        = 2.0f / 3.0f; // Weight on per-wheel force tracking error
static constexpr float W_MZ        = 1.0f / 3.0f; // Weight on yaw moment tracking error
static constexpr int   MAX_ITER    = 8;            // Gauss-Newton iterations per control cycle
static constexpr float SLIP_CLAMP  = 0.3f;         // Physical slip ratio bounds
static constexpr float MIN_HESSIAN = 1e-6f;        // Prevents division by zero in Newton step

// Wheel array index convention
static constexpr int FL = 0;
static constexpr int FR = 1;
static constexpr int RL = 2;
static constexpr int RR = 3;

// Mz sign per wheel: right-side forces create positive yaw moment
static constexpr float MZ_SIGNS[4] = { -1.0f, 1.0f, -1.0f, 1.0f };

/**
 * Build TireModel::StateInputs from shared vehicle state.
 * The vehicle state and normal load are fixed during optimization;
 * only the angular velocity changes across iterations (via trial slip ratio).
 */
[[nodiscard]] estimation::TireModel::StateInputs buildStateInputs(
    const datatypes::datatypes::VehicleState& vs,
    const float normal_load_N,
    const float wheel_angular_velocity_radps)
{
    return {
        .wheel_angular_velocity_radps = wheel_angular_velocity_radps,
        .vehicle_velocity_x_mps       = vs.v_x_mps,
        .vehicle_velocity_y_mps       = vs.v_y_mps,
        .yaw_rate_radps               = vs.yaw_rate_radps,
        .steering_angle_rad           = vs.steer_ang_rad,
        .normal_load_N                = normal_load_N,
    };
}

} // namespace

// ---- Construction ----

TorqueAllocator::TorqueAllocator(estimation::TireModel::TirePressure pressure)
    : fl_tire_(pressure, WheelSide::Left,  WheelAxle::Front),
      fr_tire_(pressure, WheelSide::Right, WheelAxle::Front),
      rl_tire_(pressure, WheelSide::Left,  WheelAxle::Rear),
      rr_tire_(pressure, WheelSide::Right, WheelAxle::Rear)
{
}

// ---- Public interface ----

float TorqueAllocator::estimateYawMoment(const datatypes::datatypes::wheel_set& f_x)
{
    // Mz = (t/2) * (Fx_right_total - Fx_left_total)
    return (TRACK_WIDTH_m * 0.5f) * ((f_x.fr + f_x.rr) - (f_x.fl + f_x.rl));
}

datatypes::datatypes::wheel_set TorqueAllocator::optimize(
    const datatypes::datatypes::wheel_set& des_f_x,
    const datatypes::datatypes::wheel_set& normal_forces_N,
    const datatypes::datatypes::wheel_set& current_slip_ratios,
    const datatypes::datatypes::VehicleState& vehicle_state,
    float acc_M_z,
    const float des_M_z)
{
    // Working copy of slip ratios — warm-started from current estimates
    float kappa[4] = {
        current_slip_ratios.fl,
        current_slip_ratios.fr,
        current_slip_ratios.rl,
        current_slip_ratios.rr,
    };

    estimation::TireModel* tires[4] = { &fl_tire_, &fr_tire_, &rl_tire_, &rr_tire_ };

    const float fz[4]     = { normal_forces_N.fl, normal_forces_N.fr,
                               normal_forces_N.rl, normal_forces_N.rr };
    const float des_fx[4] = { des_f_x.fl, des_f_x.fr, des_f_x.rl, des_f_x.rr };

    const float half_track = TRACK_WIDTH_m * 0.5f;

    for (int iter = 0; iter < MAX_ITER; ++iter)
    {
        // ---- Evaluate tire model at current trial slip ratios ----
        float acc_fx[4] = {};
        float dFdK[4]   = {};

        for (int i = 0; i < 4; ++i)
        {
            // TireModel owns the wheel-center kinematics, so the allocator only provides trial kappa + vehicle state.
            const float omega   = tires[i]->slipRatioToWheelAngularVelocity(kappa[i], vehicle_state);
            const auto  inputs  = buildStateInputs(vehicle_state, fz[i], omega);
            const auto  outputs = tires[i]->estimate(inputs);

            acc_fx[i] = outputs.longitudinal_force_N;
            dFdK[i]   = outputs.dFx_dKappa;
        }

        // ---- Yaw moment error ----
        const datatypes::datatypes::wheel_set acc_fx_set = {
            .fl = acc_fx[FL], .fr = acc_fx[FR], .rl = acc_fx[RL], .rr = acc_fx[RR] };
        const float e_Mz = acc_M_z - des_M_z;

        // ---- Gauss-Newton step per wheel ----
        //
        // Cost: J = W_FX * sum_i (Fx_i - des_fx_i)^2 + W_MZ * (Mz - des_Mz)^2
        //
        // Gradient:  dJ/dkappa_i = W_FX * e_fx_i * dFx/dkappa
        //                        + W_MZ * e_Mz   * dMz/dkappa_i
        //
        // Hessian (diagonal, Gauss-Newton approximation):
        //   H_ii = W_FX * (dFx/dkappa)^2 + W_MZ * (dMz/dkappa)^2
        //
        for (int i = 0; i < 4; ++i)
        {
            const float e_fx   = acc_fx[i] - des_fx[i];
            const float dMz_dK = MZ_SIGNS[i] * half_track * dFdK[i];

            const float grad = (W_FX * e_fx * dFdK[i]) + (W_MZ * e_Mz * dMz_dK);
            const float H_ii = (W_FX * dFdK[i] * dFdK[i]) + (W_MZ * dMz_dK * dMz_dK);

            kappa[i] -= grad / std::fmax(H_ii, MIN_HESSIAN);
            kappa[i]  = std::clamp(kappa[i], -SLIP_CLAMP, SLIP_CLAMP);
        }

        // Use the provided yaw-moment estimate to seed the first iteration, then
        // keep the internal solve self-consistent with the tire-model force updates.
        acc_M_z = estimateYawMoment(acc_fx_set);
    }

    return { .fl = kappa[FL], .fr = kappa[FR], .rl = kappa[RL], .rr = kappa[RR] };
}

} // namespace app::tv::controllers::allocator
