#pragma once

#include "controls/App_ControlLoop.h"

struct PhaseValues CalculatePwmEdges(
    const struct PhaseValues * phase_voltages,
    double                   bus_voltage);
