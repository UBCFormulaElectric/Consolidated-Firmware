#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "math.h"

typedef struct{
	double s;
	double d;
	double q;
}dqs_values;

typedef struct{
	double a;
	double b;
	double c;
}phase_values;

typedef struct{
	double prev_integral_input;
	double integral_sum;
	double gain;
	double time_const;
}controller_values;

void controlLoop(double torque_ref, double rotor_speed_ref);
