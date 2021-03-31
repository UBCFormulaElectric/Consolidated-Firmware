#pragma once

#include "controls/App_ControlLoop.h"

//double torqueControl(double omega_ref, double omega, double torque_ref, controller_values * speed_controller, bool * prev_fw_flag);
double torqueControl(double omega_ref, double omega, double torque_ref, controller_values * speed_controller, const bool * prev_fw_flag);
