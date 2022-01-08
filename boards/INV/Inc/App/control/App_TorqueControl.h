#pragma once

#include "control/App_ControlLoop.h"

// float torqueControl(float omega_ref, float omega, float torque_ref,
// ControllerValues * speed_controller, bool * prev_fw_flag);
float torqueControl(
    float                    omega_ref,
    float                    omega,
    float                    torque_ref,
    struct ControllerValues *speed_controller,
    const bool               prev_fw_flag);
