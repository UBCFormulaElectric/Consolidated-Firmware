#pragma once
#include "shared_datatypes/constants.hpp"
#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "shared_datatypes/wheel_set.hpp"

template <Decimal T> struct ControlOutput
{
    const T fl_kappa   = 0;
    const T fr_kappa   = 0;
    const T rl_kappa   = 0;
    const T rr_kappa   = 0;
    const T max_torque = 0;
    const T min_torque = 0;
};

template <Decimal T> struct ControlOutputAutonomous
{
    const T fl_kappa   = 0;
    const T fr_kappa   = 0;
    const T rl_kappa   = 0;
    const T rr_kappa   = 0;
    const T delta      = 0;
    const T max_torque = 0;
    const T min_torque = 0;
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
template <typename T>
app::tv::shared_datatypes::wheel_set<T>
    kappa_update(const app::tv::shared_datatypes::wheel_set<T> &kappas, const T v_x_mps)
{
    return {
        .fl = (1.0f + kappas.fl) * (v_x_mps / app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M),
        .fr = (1.0f + kappas.fr) * (v_x_mps / app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M),
        .rl = (1.0f + kappas.rl) * (v_x_mps / app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M),
        .rr = (1.0f + kappas.rr) * (v_x_mps / app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M),
    };
}
