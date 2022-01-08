#pragma once

#include "control/App_ControlLoop.h"

// DqsValues calculateDqsVoltages(DqsValues * dqs_ref_currents, DqsValues *
// dqs_currents, float omega, 								float bus_voltage,
// ControllerValues
// *
// id_controller, ControllerValues * iq_controller);
struct DqsValues calculateDqsVoltages(
    const struct DqsValues *const dqs_ref_currents,
    const struct DqsValues *const dqs_currents,
    const float                   omega,
    const float                   bus_voltage,
    struct ControllerValues *     id_controller,
    struct ControllerValues *     iq_controller);