#pragma once
#include "estimation/vehicle_state_estimator.hpp"
#include "shared_datatypes/datatypes.hpp"
#include "estimation/vehicle_state_estimator.hpp"

// app::tv::shared_datatypes::datatypes::VehicleState
//     estimateVehicleState();

/**
 * This is the main entrypoint into the low level vehicle controls algorithm
 * @param state The current measured vehicle state
 * @param ax Requested Longitudinal acceleration (m/s^2)
 * @param omega_dot Requested Yaw acceleration (rad/s^2)
 * @return The per-wheel torque requests to achieve the desired accelerations, in Newton-meters
 */
app::tv::shared_datatypes::datatypes::ControlOutput
    update(const app::tv::estimation::VehicleStateEstimator::Measurements &state, float ax, float omega_dot);

/**
 * This is the main entrypoint into the low level vehicle controls algorithm for autonomous
 * @param state The current measured vehicle state
 * @param ax Requested Longitudinal acceleration (m/s^2)
 * @param omega_dot Requested Yaw acceleration (rad/s^2)
 * @return
 */
app::tv::shared_datatypes::datatypes::ControlOutputAutonomous
    update_autonomous(const app::tv::estimation::VehicleStateEstimator::Measurements &state, float ax, float omega_dot);
