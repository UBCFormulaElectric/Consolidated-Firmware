#include "control/App_ParkClarkeTransform.h"
#include "control/App_ControlLoop.h"

struct PhaseValues parkClarkeTransform(
    const struct DqsValues *const dqs_voltages,
    const float                   theta)
{
    struct PhaseValues phase_voltages;

    float alpha = dqs_voltages->d * cosf(theta) - dqs_voltages->q * sinf(theta);
    float beta  = dqs_voltages->d * sinf(theta) + dqs_voltages->q * cosf(theta);

    phase_voltages.a = ((float)M_SQRT3 / 2) * alpha - beta / 2;
    phase_voltages.b = beta;
    phase_voltages.c = -((float)M_SQRT3 / 2) * alpha - beta / 2;

    return phase_voltages;
}
