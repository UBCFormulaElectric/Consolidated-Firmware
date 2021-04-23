#pragma once

#include "controls/App_ControlLoop.h"

PhaseValues calculatePwmEdges(
    const PhaseValues *const phase_voltages,
    double                   bus_voltage);
// void setPWMEdges(const PhaseValues * const phase_duration);
