#pragma once

#include "controls/App_ControlLoop.h"

// DqsValues calculateDqsVoltages(DqsValues * dqs_ref_currents, DqsValues *
// dqs_currents, double omega, 								double bus_voltage,
// ControllerValues
// *
// id_controller, ControllerValues * iq_controller);
struct DqsValues calculateDqsVoltages(
    const struct DqsValues *const dqs_ref_currents,
    const struct DqsValues *const dqs_currents,
    const double                  omega,
    const double                  bus_voltage,
    struct ControllerValues *     id_controller,
    struct ControllerValues *     iq_controller);