#include "controls/App_ParkClarkeTransform.h"
#include "controls/App_ControlLoop.h"

phase_values parkClarkeTransform(dqs_values dqs_voltages, double theta)
{
	phase_values phase_voltages;
	double alpha, beta;

	alpha = dqs_voltages.d * cos(theta) - dqs_voltages.q * sin(theta);
	beta = dqs_voltages.d * sin(theta) + dqs_voltages.q * cos(theta);

	phase_voltages.a = (sqrt(3) / 2) * alpha - beta / 2;
	phase_voltages.b = beta;
	phase_voltages.c = -(sqrt(3) / 2) * alpha - beta / 2;

	return phase_voltages;
}
