#include "control/App_ClarkeParkTransform.h"
#include "configs/App_ControlSystemConfig.h"
#include "control/App_ControlLoop.h"


struct DqsValues clarkeParkTransform(
    struct PhaseValues *const phase_currents,
    const float               theta)
{
    phase_currents->c = CALCULATE_PHC_CURRENT
                            ? -1 * (phase_currents->a + phase_currents->b)
                            : phase_currents->c;
    struct DqsValues dqs_currents;

    // Clarke Transform
    float alpha = (phase_currents->a - 0.5f * phase_currents->b -
                   0.5f * phase_currents->c) * _2_OVER_3;
    float beta = (SQRT_3_OVER_2 * phase_currents->b -
                 SQRT_3_OVER_2 * phase_currents->c) * _2_OVER_3;

    // Park Transform
    dqs_currents.d = alpha * App_ControlLoop_CosLookup(theta) + beta * App_ControlLoop_SinLookup(theta);
    dqs_currents.q = -alpha * App_ControlLoop_SinLookup(theta) + beta * App_ControlLoop_CosLookup(theta);
    dqs_currents.s = sqrtf(
        dqs_currents.d * dqs_currents.d + dqs_currents.q * dqs_currents.q);

    return dqs_currents;
}
