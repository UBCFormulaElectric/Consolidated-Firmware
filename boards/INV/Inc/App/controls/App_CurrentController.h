#pragma once

#include "controls/App_ControlLoop.h"

dqs_values calculateDQSVoltages(dqs_values dqs_ref_currents, dqs_values dqs_currents, double omega,
								double bus_voltage, controller_values * id_controller, controller_values * iq_controller);
