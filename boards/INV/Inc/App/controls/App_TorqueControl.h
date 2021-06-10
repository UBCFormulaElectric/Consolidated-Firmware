#pragma once

#include "controls/App_ControlLoop.h"

// double torqueControl(double omega_ref, double omega, double torque_ref,
// ControllerValues * speed_controller, bool * prev_fw_flag);
double torqueControl(
    double                   omega_ref,
    double                   omega,
    double                   torque_ref,
    struct ControllerValues *speed_controller,
    const bool *             prev_fw_flag);
