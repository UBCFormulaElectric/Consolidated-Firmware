#pragma once

#include "controls/App_ControlLoop.h"

void setPWMEdges(phase_values phase_duration);
phase_values calculatePWMEdges(phase_values phase_voltages, double bus_voltage);

