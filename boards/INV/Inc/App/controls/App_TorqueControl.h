#pragma once

#include "controls/App_ControlLoop.h"

double torqueControl(double omega_ref, double torque_ref, controller_values speed_controller, double omega, bool prev_fw_flag);
