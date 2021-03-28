#include "controls/App_CurrentGeneration.h"
#include "configs/App_ControlSystemConfig.h"

dqs_values generateRefCurrents(
		dqs_values dqs_ref_currents, double omega, double vdc_sensor_val, bool * fw_flag
		)
{
	dqs_values new_dqs_ref_currents;
	double id_mtpa, iq_mtpa, omega_elec_fw_thres;
	double omega_elec = omega * MOTOR_POLES / 2;
	double lamda = BACK_EMF_CONST * 2 * 9.549 / (sqrt(3) * 1000 * MOTOR_POLES);
	double vs_max = MAX_MOD_INDEX * vdc_sensor_val / sqrt(3);
	int sign_is = (dqs_ref_currents.s > 0) - (dqs_ref_currents.s < 0);

	//Generate MTPA Currents
	id_mtpa = -(lamda / (D_INDUCTANCE - Q_INDUCTANCE) + sqrt(lamda * lamda / (D_INDUCTANCE - Q_INDUCTANCE) / (D_INDUCTANCE - Q_INDUCTANCE) + 8 * dqs_ref_currents.s * dqs_ref_currents.s)) / 4;
	iq_mtpa = (dqs_ref_currents.s * dqs_ref_currents.s - id_mtpa * id_mtpa > 0) ? sign_is * sqrt(dqs_ref_currents.s * dqs_ref_currents.s - id_mtpa * id_mtpa) : 0;

	//Calculate FW Speed Threshold
	omega_elec_fw_thres = vs_max / sqrt((D_INDUCTANCE * id_mtpa + lamda) * (D_INDUCTANCE * id_mtpa + lamda) + (Q_INDUCTANCE * iq_mtpa) * (Q_INDUCTANCE * iq_mtpa));

	//If in FW Region, Re-Calculate Ref Currents
	if(fabs(omega_elec) > omega_elec_fw_thres)
	{
		*fw_flag = 1;
		dqs_ref_currents.d = (-lamda * D_INDUCTANCE +
				sqrt((lamda * lamda * D_INDUCTANCE * D_INDUCTANCE) -
				(D_INDUCTANCE * D_INDUCTANCE - Q_INDUCTANCE * Q_INDUCTANCE) * (lamda * lamda + Q_INDUCTANCE * Q_INDUCTANCE * dqs_ref_currents.s * dqs_ref_currents.s -
				vs_max * vs_max / omega_elec / omega_elec))) / (D_INDUCTANCE * D_INDUCTANCE - Q_INDUCTANCE * Q_INDUCTANCE);

		dqs_ref_currents.q = (dqs_ref_currents.s * dqs_ref_currents.s - dqs_ref_currents.d * dqs_ref_currents.d > 0) ? sign_is * sqrt(dqs_ref_currents.s * dqs_ref_currents.s - dqs_ref_currents.d * dqs_ref_currents.d) : 0;
	}
	else
	{
		*fw_flag = 0;
		dqs_ref_currents.d = id_mtpa;
		dqs_ref_currents.q = iq_mtpa;
	}

	new_dqs_ref_currents.d = dqs_ref_currents.d;
	new_dqs_ref_currents.q = dqs_ref_currents.q;
	new_dqs_ref_currents.s = sqrt(dqs_ref_currents.d * dqs_ref_currents.d + dqs_ref_currents.q * dqs_ref_currents.q);

	return new_dqs_ref_currents;
}

