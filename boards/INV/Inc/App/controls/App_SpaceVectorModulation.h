#pragma once

#include "controls/App_ControlLoop.h"

phase_values calculatePwmEdges(const phase_values * const phase_voltages, double bus_voltage);
//void setPWMEdges(const phase_values * const phase_duration);
