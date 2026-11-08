#pragma once
#include "shared_datatypes/constants.hpp"
#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "shared_datatypes/wheel_set.hpp"
#include "controllers/controllers_config.hpp"
#include "controllers/speed_controller.hpp"
#include "controllers/power_limiting/power_limiting.hpp"
#include "tv_debug.h"
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
 * Given the slip ratio setpoints, computes the motor speed setpoints that achieve them at the current vehicle speed
 * and runs the per-wheel speed controller (with the tire force feedforward) to track them, then applies the battery
 * power limits. The speed controllers are reset while the car is stopped. While the driver brakes, the net motor
 * torque is limited to 0 or less.
 * @param control slip ratio setpoints (kappa = (wheel_speed - wheel_ground_speed) / wheel_ground_speed) and per-wheel
 * torque limits in Nm, from update()
 * @param state current vehicle state: estimated v_x / v_y in m/s and yaw rate (wheel speeds over ground, slip angles),
 * body accelerations in m/s^2 (ISO 8855, y left) for the wheel loads' load transfer, measured wheel speeds, steer
 * angles and the brake pedal
 * @param discharge_pl power limit when discharging the battery pack in W, from the BMS
 * @param charge_pl power limit when charging the battery pack (regen) in W, from the BMS
 * @return per-wheel motor torque requests in Nm
 */
template <Decimal T>
app::tv::shared_datatypes::wheel_set<T> kappa_update(
    const ControlOutput<T>                           &control,
    const app::tv::shared_datatypes::VehicleState<T> &state,
    T                                                 discharge_pl,
    T                                                 charge_pl);
