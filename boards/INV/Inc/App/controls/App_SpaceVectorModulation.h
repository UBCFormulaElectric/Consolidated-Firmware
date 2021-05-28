#pragma once

#include "controls/App_ControlLoop.h"

PhaseValues CalculatePwmEdges(
    const PhaseValues * phase_voltages,
    double                   bus_voltage);
void SetPwmEdges(const PhaseValues * phase_duration);
