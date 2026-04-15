#pragma once
#include "shared_datatypes/constants.hpp"
#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "shared_datatypes/wheel_set.hpp"
// #include <iostream>

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
template <typename T>
app::tv::shared_datatypes::wheel_set<T>
    kappa_update(const app::tv::shared_datatypes::wheel_set<T> &kappas, const T v_x_mps)
{
    // std::cout << "Vx: " << v_x_mps << std::endl;
    // std::cout << "Kappas: " << kappas.fl << kappas.fr << kappas.rl << kappas.rr << std::endl;

    T v_x_mps_capped = std::max(v_x_mps, static_cast<T>(1));

    return {
        .fl = GEAR_RATIO * (static_cast<T>(1) + kappas.fl) * (v_x_mps_capped / static_cast<T>(app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M)),
        .fr = GEAR_RATIO * (static_cast<T>(1) + kappas.fr) * (v_x_mps_capped / static_cast<T>(app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M)),
        .rl = GEAR_RATIO * (static_cast<T>(1) + kappas.rl) * (v_x_mps_capped / static_cast<T>(app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M)),
        .rr = GEAR_RATIO * (static_cast<T>(1) + kappas.rr) * (v_x_mps_capped / static_cast<T>(app::tv::shared_datatypes::vd_constants::WHEEL_RADIUS_M)),
    };
}
