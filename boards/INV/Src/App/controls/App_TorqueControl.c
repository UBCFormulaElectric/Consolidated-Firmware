#include "controls/App_TorqueControl.h"
#include "controls/App_PIController.h"
#include "configs/App_ControlSystemConfig.h"

/*
 * Generates the torque setpoint for the inverter, based on the speed control loop
 * and torque setpoint requested by firmware. The output will be whichever setpoint
 * is lower, ensuring the inverter does not violate MAX_MOTOR_SPEED.
 */

double torqueControl(double omega_ref, const double omega, const double torque_ref, controller_values * speed_controller, const bool * const prev_fw_flag)
{
	double omega_torque_output, torque_output, stator_current_output;

	// Make omega_ref have the same sign as torque_ref
	omega_ref = omega_ref * (int8_t)((torque_ref > 0) - (torque_ref < 0));
	int8_t direction_sign = (int8_t)((omega > 0) - (omega < 0));
	int8_t torque_sign    = (int8_t)((torque_ref > 0) - (torque_ref < 0));

	// calculate torque output with PI controller
	omega_torque_output = calculatePiOutputs(speed_controller, omega_ref, omega,
										  MAX_MOTOR_TORQUE, 0);

	// take smaller of omega_ref and torque_ref
	torque_output = (fabs(omega_torque_output) < fabs(torque_ref))
						? omega_torque_output
						: torque_ref;

	// if operating in field weakening region, take power limit
	if (prev_fw_flag)
	{
		torque_output = (fabs(MAX_MOTOR_POWER / (omega + 0.01)) < fabs(torque_output))
							? torque_sign * MAX_MOTOR_POWER / (fabs(omega) + 0.01)
							: torque_output;
	}
	// if operating in regen region
	if (direction_sign != torque_sign)
	{
		torque_output = (fabs(torque_output) > REGEN_POWER_LIMIT / (omega + 0.01))
							? REGEN_POWER_LIMIT * torque_sign / (omega + 0.01)
							: torque_output;
	}
	// convert torque to stator current, and limit stator current to Ismax
	if (torque_output / TORQUE_CONST < MAX_STATOR_CURRENT &&
		torque_output / TORQUE_CONST > -MAX_STATOR_CURRENT)
	{
		stator_current_output = torque_output / TORQUE_CONST;
	}
	else
	{
		stator_current_output =
			(torque_output / TORQUE_CONST >= 0) ? MAX_STATOR_CURRENT : -MAX_STATOR_CURRENT;
	}
	return stator_current_output;
}
