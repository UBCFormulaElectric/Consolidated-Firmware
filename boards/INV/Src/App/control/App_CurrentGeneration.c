#include "control/App_CurrentGeneration.h"
#include "configs/App_ControlSystemConfig.h"

struct DqsValues generateRefCurrents(
    const struct DqsValues *const dqs_ref_currents,
    const float                   omega,
    const float                   vdc_sensor_val,
    bool *const                   fw_flag)
{
    struct DqsValues new_dqs_ref_currents;
    float            id_mtpa, iq_mtpa, omega_elec_fw_thres;
    float            omega_elec = omega * MOTOR_POLES / 2;
    float            lamda =
        BACK_EMF_CONST * 2 * 9.549f / ((float)M_SQRT3 * 1000 * MOTOR_POLES);
    float vs_max  = MAX_MOD_INDEX * vdc_sensor_val / (float)M_SQRT3;
    int   sign_is = (dqs_ref_currents->s > 0) - (dqs_ref_currents->s < 0);

    // Generate MTPA Currents
    id_mtpa = -(lamda / (D_INDUCTANCE - Q_INDUCTANCE) +
                sqrtf(
                    lamda * lamda / (D_INDUCTANCE - Q_INDUCTANCE) /
                        (D_INDUCTANCE - Q_INDUCTANCE) +
                    8 * dqs_ref_currents->s * dqs_ref_currents->s)) /
              4;
    iq_mtpa =
        (dqs_ref_currents->s * dqs_ref_currents->s - id_mtpa * id_mtpa > 0)
            ? (float)sign_is * sqrtf(
                                   dqs_ref_currents->s * dqs_ref_currents->s -
                                   id_mtpa * id_mtpa)
            : 0;

    // Calculate FW Speed Threshold
    omega_elec_fw_thres =
        vs_max / sqrtf(
                     (D_INDUCTANCE * id_mtpa + lamda) *
                         (D_INDUCTANCE * id_mtpa + lamda) +
                     (Q_INDUCTANCE * iq_mtpa) * (Q_INDUCTANCE * iq_mtpa));

    // If in FW Region, Re-Calculate Ref Currents
    if (fabsf(omega_elec) > omega_elec_fw_thres)
    {
        *fw_flag = 1;
        new_dqs_ref_currents.d =
            (-lamda * D_INDUCTANCE +
             sqrtf(
                 (lamda * lamda * D_INDUCTANCE * D_INDUCTANCE) -
                 (D_INDUCTANCE * D_INDUCTANCE - Q_INDUCTANCE * Q_INDUCTANCE) *
                     (lamda * lamda +
                      Q_INDUCTANCE * Q_INDUCTANCE * dqs_ref_currents->s *
                          dqs_ref_currents->s -
                      vs_max * vs_max / omega_elec / omega_elec))) /
            (D_INDUCTANCE * D_INDUCTANCE - Q_INDUCTANCE * Q_INDUCTANCE);

        new_dqs_ref_currents.q =
            (dqs_ref_currents->s * dqs_ref_currents->s -
                 dqs_ref_currents->d * dqs_ref_currents->d >
             0)
                ? (float)sign_is *
                      sqrtf(
                          dqs_ref_currents->s * dqs_ref_currents->s -
                          dqs_ref_currents->d * dqs_ref_currents->d)
                : 0;
    }
    else
    {
        *fw_flag               = 0;
        new_dqs_ref_currents.d = id_mtpa;
        new_dqs_ref_currents.q = iq_mtpa;
    }

    new_dqs_ref_currents.s = sqrtf(
        dqs_ref_currents->d * dqs_ref_currents->d +
        dqs_ref_currents->q * dqs_ref_currents->q);

    return new_dqs_ref_currents;
}
