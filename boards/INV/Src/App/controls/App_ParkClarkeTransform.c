#include "controls/App_ParkClarkeTransform.h"
#include "controls/App_ControlLoop.h"

phase_values parkClarkeTransform(const dqs_values * const dqs_voltages, const double theta)
{
	phase_values phase_voltages;

	double alpha = dqs_voltages->d * cos(theta) - dqs_voltages->q * sin(theta);
	double beta = dqs_voltages->d * sin(theta) + dqs_voltages->q * cos(theta);

	phase_voltages.a = (sqrt(3) / 2) * alpha - beta / 2;
	phase_voltages.b = beta;
	phase_voltages.c = -(sqrt(3) / 2) * alpha - beta / 2;

	return phase_voltages;
}
