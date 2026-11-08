#pragma once
#include "controls/params.hpp"
#include "controls/datatypes/vehicle_state.hpp"
#include "controls/datatypes/wheel_set.hpp"
#include "controls/actuation/speed_controller.hpp"
#include "controls/actuation/power_limit.hpp"
#include "controls/controls_debug.h"
#include "controls/estimation/tire_model.hpp"
#include <algorithm>
#include <cmath>

namespace app::controls
{
template <Decimal T> struct ControlOutput
{
    wheel_set<T> kappas; // slip ratio setpoints
    wheel_set<T> torque_max_Nm;
    wheel_set<T> torque_min_Nm;
};

template <Decimal T> struct ControlOutputAutonomous
{
    wheel_set<T> kappas;
    wheel_set<T> torque_max_Nm;
    wheel_set<T> torque_min_Nm;
    const T      steering_wheel_angle_rad = 0;
};

struct ControlsConfig
{
    // TODO: add other controls related configs here like drive mode
    bool regen_enabled = false;
};

/**
 * This is the main entrypoint into the low level vehicle controls algorithm
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return The per-wheel torque requests to achieve the desired accelerations, in Newton-meters
 */
template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state, const ControlsConfig config);

/**
 * This is the main entrypoint into the low level vehicle controls algorithm for autonomous
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return
 */
template <Decimal T>
ControlOutputAutonomous<T> update_autonomous(const VehicleState<T> &state, const ControlsConfig config);

/**
 * Given the slip ratio setpoints, computes the motor speed setpoints that achieve them at the current vehicle speed
 * and runs the per-wheel speed controller (with the tire force feedforward) to track them, then applies the battery
 * power limits. The speed controllers are reset while the car is stopped. While the driver brakes, the net motor
 * torque is limited to 0 or less.
 * @param control slip ratio setpoints (kappa = (wheel_speed - wheel_ground_speed) / wheel_ground_speed) and per-wheel
 * torque limits in Nm, from update()
 * @param state current vehicle state: estimated v_x / v_y in m/s and yaw rate (wheel speeds over ground, slip angles),
 * body accelerations in m/s^2 (ISO 8855, y left) for the wheel loads' load transfer, measured wheel speeds, steer
 * angles and the brake pedal
 * @return per-wheel motor torque requests in Nm
 */
template <Decimal T>
wheel_set<T> kappa_update(const ControlOutput<T> &control, const VehicleState<T> &state, const ControlsConfig config);
} // namespace app::controls
