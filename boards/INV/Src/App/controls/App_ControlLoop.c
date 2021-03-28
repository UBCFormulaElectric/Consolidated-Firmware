#include "controls/App_ControlLoop.h"
#include "main.h"
#include "configs/App_ControlSystemConfig.h"
#include "controls/App_ClarkeParkTransform.h"
#include "controls/App_ParkClarkeTransform.h"
#include "controls/App_CurrentController.h"
#include "controls/App_AdaptionGain.h"
#include "controls/App_CurrentGeneration.h"
#include "controls/App_PIController.h"
#include "controls/App_SpaceVectorModulation.h"
#include "controls/App_TorqueControl.h"

/*
 * This file executes the motor control system functionality
 * in the correct order. It will be executed during the
 * interrupt callback occuring at the control loop frequency
 * of SAMPLE_FREQUENCY, defined in ControlSystemConfig.h
 *
 * The control scheme is as follows:
 *
 * 1. Sample Sensor Values:
 * 		-IPHA, IPHB, IPHC
 * 		-Rotor Position
 * 		-DC Bus Voltage
 * 2. Calculate Speed of Rotor
 * 2. Calculate ClarkePark Transform on phase currents
 *
 * 3. Read Torque Request sent over CAN from DCM
 * 4. Calculate Torque Setpoint with TorqueControl.c
 * 5. Calculate Id and Iq with CurrentGeneration.c
 * 6. Calculate Adaptive Gain (if enabled)
 * 7. Calculate Current Controller Outputs (Vd, Vq)
 * 8. Calculate ParkClarke Transform on Vd, Vq
 * 9. Calculate PWM edges for next cycle using SpaceVectorModulation.c
 * 10. Write PWM edge values to registers
 *
 * Note: Set GPIO pin high at beginning of function, set low at end
 * to measure total time of execution
 */

//INITIALIZE VARIABLES HERE
phase_values phase_voltages;
phase_values phase_currents;
phase_values phase_duration;
dqs_values dqs_ref_currents;
dqs_values dqs_currents;
dqs_values dqs_voltages;

controller_values speed_controller =
{
		.prev_integral_input = 0,
		.integral_sum = 0,
		.gain = SPEED_GAIN,
		.time_const = SPEED_TIME_CONST,
};

controller_values id_controller =
{
		.prev_integral_input = 0,
		.integral_sum = 0,
		.gain = D_GAIN,
		.time_const = D_TIME_CONST
};

controller_values iq_controller =
		{
				iq_controller.prev_integral_input = 0;
		iq_controller.integral_sum = 0;
		iq_controller.gain = Q_GAIN;
		iq_controller.time_const = Q_TIME_CONST;
		};

double rotor_position = 0;
double prev_rotor_position = 0;
double rotor_speed = 0;
double bus_voltage = 0;
double torque_ref = 0;
bool fw_flag = 0;
bool prev_fw_flag = 0;






void ControlLoop() {
	phase_currents.pha = powerstage.getCurrent("A");
	phase_currents.phb = powerstage.getCurrent("B");
	phase_currents.phc = powerstage.getCurrent("C");

	rotor_position = motor.getPosition();
	bus_voltage = powerstage.getVoltage();
	torque_ref = can_rx.getTorqueRequest("DCM");
	speed_ref = MAX_MOTOR_SPEED;
	rotor_speed = (rotor_position - prev_rotor_position) * SAMPLE_FREQUENCY;

	dqs_currents = clarkeParkTransform(phase_currents, rotor_position);

	//Get stator current reference
	dqs_ref_currents.s = torqueControl(speed_ref, rotor_speed, torque_ref,
									speed_controller, prev_fw_flag);

	//If used, calculate adaption gains
	if(ADAPTION_GAIN_ENABLED)
	{
		id_controller = adaptionGain(id_controller, dqs_ref_currents.s);
		iq_controller = adaptionGain(iq_controller, dqs_ref_currents.s);
	}

	//Calculate d/q current references
	dqs_ref_currents = generateRefCurrents(
			dqs_ref_currents, rotor_speed, bus_voltage, &fw_flag);

	dqs_voltages = calculateDQSVoltages(dqs_ref_currents, dqs_currents, rotor_speed, bus_voltage, &id_controller, &iq_controller);

	phase_voltages = parkClarkeTransform(dqs_voltages, rotor_speed);

	phase_duration = calculatePWMEdges(phase_voltages, bus_voltage);
	//TODO not implemented yet
	//setPWMEdges(phase_duration);

	prev_fw_flag = fw_flag;
	prev_rotor_position = rotor_position;

}









