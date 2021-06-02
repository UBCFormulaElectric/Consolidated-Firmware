#include "controls/App_ClarkeParkTransform.h"
#include "configs/App_ControlSystemConfig.h"
#include <math.h>
//#include <arm_math.h>

DqsValues clarkeParkTransform(
    struct PhaseValues *const phase_currents,
    const double              theta)
{
    phase_currents->c = CALCULATE_PHC_CURRENT
                            ? (-1 * (phase_currents->a + phase_currents->b))
                            : phase_currents->c;
    DqsValues dqs_currents;

    // Clarke Transform
    double alpha = (phase_currents->a - 0.5 * phase_currents->b -
                    0.5 * phase_currents->c) *
                   2 / 3;
    double beta = ((sqrt(3) / 2) * phase_currents->b -
                   (sqrt(3) / 2) * phase_currents->c) *
                  2 / 3;

    // Park Transform
    dqs_currents.d = alpha * cos(theta) + beta * sin(theta);
    dqs_currents.q = -alpha * sin(theta) + beta * cos(theta);
    dqs_currents.s =
        sqrt(dqs_currents.d * dqs_currents.d + dqs_currents.q * dqs_currents.q);

    return dqs_currents;
}
