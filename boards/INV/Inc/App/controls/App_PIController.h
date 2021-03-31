#pragma once

#include "controls/App_ControlLoop.h"

double calculatePiOutputs(
		controller_values * pi_values,
		double ref,
		double actual,
		double limit,
		double comp);
