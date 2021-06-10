#include "controls/App_ParkClarkeTransform.h"
#include "controls/App_ControlLoop.h"

struct PhaseValues parkClarkeTransform(
    const struct DqsValues *const dqs_voltages,
    const double                  theta)
{
    struct PhaseValues phase_voltages;

    double alpha = dqs_voltages->d * cos(theta) - dqs_voltages->q * sin(theta);
    double beta  = dqs_voltages->d * sin(theta) + dqs_voltages->q * cos(theta);

    phase_voltages.a = (sqrt(3) / 2) * alpha - beta / 2;
    phase_voltages.b = beta;
    phase_voltages.c = -(sqrt(3) / 2) * alpha - beta / 2;

    return phase_voltages;
}
