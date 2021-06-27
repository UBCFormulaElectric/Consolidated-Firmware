#include "controls/App_CurrentController.h"
#include "configs/App_ControlSystemConfig.h"
#include "controls/App_PIController.h"

struct DqsValues calculateDqsVoltages(
    const struct DqsValues *const dqs_ref_currents,
    const struct DqsValues *const dqs_currents,
    const float                   omega,
    const float                   bus_voltage,
    struct ControllerValues *     id_controller,
    struct ControllerValues *     iq_controller)
{
    struct DqsValues dqs_voltages, dqs_comp_voltages;
    float            vq_limit;
    float            lamda =
        BACK_EMF_CONST * 2 * 9.549f / ((float)M_SQRT3 * 1000 * MOTOR_POLES);
    float vs_max     = 0.9f * bus_voltage / (float)M_SQRT3;
    float omega_elec = omega * MOTOR_POLES / 2;

    // Set voltage compensation terms if D/Q decoupling is enabled
    if (DECOUPLING_ENABLED)
    {
        dqs_comp_voltages.d = omega_elec * dqs_currents->q * Q_INDUCTANCE * -1;
        dqs_comp_voltages.q =
            omega_elec * (dqs_currents->d * D_INDUCTANCE + lamda);
    }
    else
    {
        dqs_comp_voltages.d = 0;
        dqs_comp_voltages.q = 0;
    }
    dqs_comp_voltages.s = sqrtf(
        dqs_comp_voltages.d * dqs_comp_voltages.d +
        dqs_comp_voltages.q * dqs_comp_voltages.q);

    // calculate vd and vq PI controller output
    dqs_voltages.d = calculatePiOutputs(
        id_controller, dqs_ref_currents->d, dqs_currents->d, vs_max,
        dqs_comp_voltages.d);
    vq_limit       = sqrtf(vs_max * vs_max - dqs_voltages.d * dqs_voltages.d);
    dqs_voltages.q = calculatePiOutputs(
        iq_controller, dqs_ref_currents->q, dqs_currents->q, vq_limit,
        dqs_comp_voltages.q);
    dqs_voltages.s = sqrtf(
        dqs_voltages.d * dqs_voltages.d + dqs_voltages.q * dqs_voltages.q);

    return dqs_voltages;
}
