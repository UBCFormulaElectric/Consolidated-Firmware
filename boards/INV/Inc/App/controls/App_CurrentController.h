#pragma once

#include "controls/App_ControlLoop.h"

//dqs_values calculateDqsVoltages(dqs_values * dqs_ref_currents, dqs_values * dqs_currents, double omega,
//								double bus_voltage, controller_values * id_controller, controller_values * iq_controller);
dqs_values calculateDqsVoltages(const dqs_values * const dqs_ref_currents, const dqs_values * const dqs_currents, const double omega,
								const double bus_voltage, controller_values * id_controller, controller_values * iq_controller);