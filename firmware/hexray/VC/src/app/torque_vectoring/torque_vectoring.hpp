#pragma once
#include "estimation/vehicle_state_estimator.hpp"
#include "shared_datatypes/datatypes.hpp"

/**
 * This is the main entrypoint into the low level vehicle controls algorithm
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return The per-wheel torque requests to achieve the desired accelerations, in Newton-meters
 */
app::tv::shared_datatypes::ControlOutput update(const app::tv::estimation::Measurements &state);

/**
 * This is the main entrypoint into the low level vehicle controls algorithm for autonomous
 * @param state The current measured vehicle state, note that intent is in here as well
 * @return
 */
app::tv::shared_datatypes::ControlOutputAutonomous update_autonomous(const app::tv::estimation::Measurements &state);
