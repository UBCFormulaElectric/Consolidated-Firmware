#pragma once

#include "control/App_ControlLoop.h"

struct PhaseValues CalculatePwmEdges(
    const struct PhaseValues *phase_voltages,
    float                     bus_voltage);
