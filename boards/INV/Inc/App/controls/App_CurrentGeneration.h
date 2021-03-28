#pragma once

#include "controls/App_ControlLoop.h"

dqs_values generateRefCurrents(dqs_values dqs_ref_currents, double omega, double vdc_sensor_val, bool * fw_flag);
