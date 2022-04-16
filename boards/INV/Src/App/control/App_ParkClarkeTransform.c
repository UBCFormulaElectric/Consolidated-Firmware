#include "control/App_ParkClarkeTransform.h"
#include "control/App_ControlLoop.h"

struct PhaseValues parkClarkeTransform(
    const struct DqsValues *const dqs_voltages,
    const float                   theta)
{
    struct PhaseValues phase_voltages;

    float alpha = dqs_voltages->d * App_ControlLoop_CosLookup(theta) - dqs_voltages->q * App_ControlLoop_SinLookup(theta);
    float beta  = dqs_voltages->d * App_ControlLoop_SinLookup(theta) + dqs_voltages->q * App_ControlLoop_CosLookup(theta);

    phase_voltages.a = ((float)M_SQRT3 / 2) * alpha - beta / 2;
    phase_voltages.b = beta;
    phase_voltages.c = -((float)M_SQRT3 / 2) * alpha - beta / 2;

    return phase_voltages;
}
