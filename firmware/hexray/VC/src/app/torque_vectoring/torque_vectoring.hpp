#pragma once
#include "shared_datatypes/constants.hpp"
#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "shared_datatypes/wheel_set.hpp"
#include "estimation/tire_model.hpp"
#include <algorithm>
#include <cmath>

template <Decimal T> struct ControlOutput
{
    app::tv::shared_datatypes::wheel_set<T> kappas;
    app::tv::shared_datatypes::wheel_set<T> torque_max;
    app::tv::shared_datatypes::wheel_set<T> torque_min;
};

template <Decimal T> struct ControlOutputAutonomous
{
    app::tv::shared_datatypes::wheel_set<T> kappas;
    app::tv::shared_datatypes::wheel_set<T> torque_max;
    app::tv::shared_datatypes::wheel_set<T> torque_min;
    const T                                 delta = 0;
};

/**
 * This is the main entrypoint into the low level vehicle controls algorithm
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return The per-wheel torque requests to achieve the desired accelerations, in Newton-meters
 */
template <Decimal T> ControlOutput<T> update(const app::tv::shared_datatypes::VehicleState<T> &state);

/**
 * This is the main entrypoint into the low level vehicle controls algorithm for autonomous
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return
 */
template <Decimal T>
ControlOutputAutonomous<T> update_autonomous(const app::tv::shared_datatypes::VehicleState<T> &state);

/**
 * Given a slip ratio setpopint, gives wheel angular velocity setpoints to achieve that slip ratio at the current
 * vehicle speed
 * @param kappas slip ratio setpoints for each wheel, where kappa = (wheel_speed - vehicle_speed) / vehicle_speed
 * @param v_x_mps current longitudinal vehicle speed in meters per second
 * @return wheel velocity setpoints
 */
template <Decimal T>
app::tv::shared_datatypes::wheel_set<T>
    kappa_update(const app::tv::shared_datatypes::wheel_set<T> &kappas, const T v_x_mps)
{
    const T v_regularized = std::max(
        std::abs(v_x_mps), static_cast<T>(app::tv::shared_datatypes::vd_constants::SLIP_REGULARIZATION_SPEED_MPS));

    // Slip is defined against the effective rolling radius, not the unloaded radius. Only v_x is available at this
    // rate, so the loads come from static weight + aero (no load transfer, which moves Re by ~0.4% at 1.5 g).
    const auto [fz_fl, fz_fr, fz_rl, fz_rr] = app::tv::shared_datatypes::VehicleState<T>{ .v_x_mps = v_x_mps }.est_Fz_N();

    const auto motor_speed_request = [&](const T kappa, const T fz_N)
    {
        // Reverse is not a supported mode: never request backwards wheel rotation.
        const T wheel_surface_speed_request = std::max(v_x_mps + kappa * v_regularized, T(0));
        const T rolling_radius_m =
            app::tv::estimation::tire_model.effectiveRollingRadius_m(static_cast<float>(fz_N), std::abs(v_x_mps));
        return static_cast<T>(GEAR_RATIO) * wheel_surface_speed_request / rolling_radius_m;
    };

    return {
        .fl = motor_speed_request(kappas.fl, fz_fl),
        .fr = motor_speed_request(kappas.fr, fz_fr),
        .rl = motor_speed_request(kappas.rl, fz_rl),
        .rr = motor_speed_request(kappas.rr, fz_rr),
    };
}
