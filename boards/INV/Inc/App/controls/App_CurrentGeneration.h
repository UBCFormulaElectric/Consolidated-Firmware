#pragma once

#include "controls/App_ControlLoop.h"

dqs_values generateRefCurrents(
		const dqs_values * const dqs_ref_currents, double omega,
		double vdc_sensor_val, bool * fw_flag);
