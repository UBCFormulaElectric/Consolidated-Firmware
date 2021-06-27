#include "controls/App_ClarkeParkTransform.h"
#include "configs/App_ControlSystemConfig.h"
#include <arm_math.h>
#include <math.h>

struct DqsValues clarkeParkTransform(
    struct PhaseValues *const phase_currents,
    const float               theta)
{
    phase_currents->c = CALCULATE_PHC_CURRENT
                            ? (-1 * (phase_currents->a + phase_currents->b))
                            : phase_currents->c;
    struct DqsValues dqs_currents;

    // Clarke Transform
    float alpha = (phase_currents->a - 0.5 * phase_currents->b -
                   0.5 * phase_currents->c) *
                  2 / 3;
    float beta = (M_SQRT3 / 2) * phase_currents->b -
                 (M_SQRT3 / 2) * phase_currents->c * 2 / 3;

    // Park Transform
    dqs_currents.d = alpha * cosf(theta) + beta * sinf(theta);
    dqs_currents.q = -alpha * sinf(theta) + beta * cosf(theta);
    dqs_currents.s = sqrtf(
        dqs_currents.d * dqs_currents.d + dqs_currents.q * dqs_currents.q);

    return dqs_currents;
}
