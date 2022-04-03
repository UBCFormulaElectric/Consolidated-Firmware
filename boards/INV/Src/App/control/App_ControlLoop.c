#include "control/App_ControlLoop.h"
#include "main.h"
#include "configs/App_ControlSystemConfig.h"
#include "control/App_ClarkeParkTransform.h"
#include "control/App_ParkClarkeTransform.h"
#include "control/App_CurrentController.h"
#include "control/App_AdaptionGain.h"
#include "control/App_CurrentGeneration.h"
#include "control/App_SpaceVectorModulation.h"
#include "control/App_TorqueControl.h"
#include "lookup_tables/App_MotorLutInterface.h"
#include "control/App_PIController.h"
//#include "states/App_AllStates.h"
#include "App_PowerStage.h"
#include "App_Motor.h"
#include "App_Faults.h"

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

// INITIALIZE VARIABLES HERE
static struct PhaseValues phase_voltages, phase_currents, phase_duration;
static struct DqsValues   dqs_ref_currents, dqs_currents, dqs_voltages;

static struct ControllerValues speed_controller = {
    .prev_integral_input = 0,
    .integral_sum        = 0,
    .output              = 0,
    .gain                = SPEED_GAIN,
    .time_const          = SPEED_TIME_CONST,
};

static struct ControllerValues id_controller = { .prev_integral_input = 0,
                                                 .integral_sum        = 0,
                                                 .output              = 0,
                                                 .gain                = D_GAIN,
                                                 .time_const = D_TIME_CONST };

static struct ControllerValues iq_controller = { .prev_integral_input = 0,
                                                 .integral_sum        = 0,
                                                 .output              = 0,
                                                 .gain                = Q_GAIN,
                                                 .time_const = Q_TIME_CONST };

static float   rotor_position                   = 0;
static float   prev_rotor_position              = 0;
static float   rotor_speed                      = 0;
static float   prev_rotor_speed                 = 0;
static float   motor_temp                       = 0.0f;
static float   powerstage_temp                  = 0.0f;
static float   motor_derated_current_limit      = 0.0f;
static float   powerstage_derated_current_limit = 0.0f;
static float   stator_current_limit             = 0.0f;
static float   bus_voltage                      = 0;
static float   mod_index                        = 0;
static float   phc_current_calc                 = 0;
static float   torque_ref                       = 0;
static bool    fw_flag                          = 0;
static bool    prev_fw_flag;
static uint8_t mode            = MODE_UNDEFINED;
static uint8_t prev_mode       = MODE_UNDEFINED;
static uint8_t mc_mode = TORQUE_CONTROL;
static uint8_t prev_mc_mode = TORQUE_CONTROL;
static uint8_t state = CANMSGS_INV_STATE_STATE_COMMAND_INIT_CHOICE;
static uint8_t prev_state = CANMSGS_INV_STATE_STATE_COMMAND_INIT_CHOICE;
static float   rotor_speed_ref = 0;
static float   mod_index_ref   = 0;
static float   ph_cur_peak_ref = 0;
static float   fund_freq_ref   = 0;

void App_ControlLoop_Run(const struct InvWorld *world)
{
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);
    struct InvCanRxInterface *can_rx      = App_InvWorld_GetCanRx(world);
    struct InvCanTxInterface *can_tx      = App_InvWorld_GetCanTx(world);

    //Retrieve position data from previous cycle
    rotor_position = App_Motor_GetPosition();

    //Start getting position for next cycle
    App_Motor_StartGetPosition();

    // Get user requests from the CAN bus
    rotor_speed_ref =
        App_CanRx_INV_ROTOR_SPEED_REQ_GetSignal_ROTOR_SPEED_REQ(can_rx);
    mode = App_CanRx_INV_STATE_REQ_GetSignal_MODE_REQ(can_rx);
    mod_index_ref =
        App_CanRx_INV_MOD_INDEX_REQ_GetSignal_MOD_INDEX_REQ(can_rx) / 100.0f;

    fund_freq_ref =
            App_CanRx_INV_FUND_FREQ_REQ_GetSignal_FUND_FREQ_REQ(can_rx);
    if (fund_freq_ref < 0 ||
        fund_freq_ref > MAX_MOTOR_SPEED / 60.0f * (MOTOR_POLES / 2.0f))
    {
        App_GateDrive_Shutdown(gate_drive);
        App_CanTx_SetPeriodicSignal_MC_FUND_FREQ_REQ_IMPLAUSIBLE(
                can_tx, 1);
    }

    mc_mode = App_CanRx_INV_STATE_REQ_GetSignal_TORQUE_SPEED_MODE_REQ(can_rx);

    torque_ref = App_CanRx_DCM_TORQUE_REQUEST_REAR_GetSignal_RL_TORQUE_REQUEST(can_rx);

    if (fabsf(torque_ref) > MAX_MOTOR_TORQUE) {
        App_GateDrive_Shutdown(gate_drive);
        App_CanTx_SetPeriodicSignal_MC_TORQUE_REQUEST_IMPLAUSIBLE(
                can_tx, 1);
    }

    // Calculate derated current limits based on temperature
    motor_derated_current_limit      = App_Motor_GetDeratedCurrent();
    powerstage_derated_current_limit = App_PowerStage_GetDeratedCurrent();

    // Choose the lower of the derated currents as the maximum stator current
    stator_current_limit = motor_derated_current_limit <
                      powerstage_derated_current_limit ?
                      motor_derated_current_limit :
                      powerstage_derated_current_limit;

    if (App_Faults_FaultedMotorShutdown(can_tx))
    {
        mode = MODE_UNDEFINED;
    }

    // TODO Delete these lines and add them to EXTI section in main after board
    // revision
    else if (App_GateDrive_GetPhbLoDiag(gate_drive))
    {
        App_CanTx_SetPeriodicSignal_PHB_LO_DIAG(can_tx, 1);
        mode = MODE_UNDEFINED;
    }
    else if (App_PowerStage_GetOTFault(power_stage))
    {
        App_CanTx_SetPeriodicSignal_PWRSTG_OT_ALARM(can_tx, 1);
        mode = MODE_UNDEFINED;
    }
    else if (App_Motor_GetOTAlarm())
    {
        App_CanTx_SetPeriodicSignal_MOTOR_OT_ALARM(can_tx, 1);
        mode = MODE_UNDEFINED;
    }

    App_CanTx_SetPeriodicSignal_MODE(can_tx, mode);
    state = App_CanTx_GetPeriodicSignal_STATE(can_tx);

    App_PowerStage_GetPhaseCurrents(power_stage, &phase_currents);
    phc_current_calc = -1 * (phase_currents.a + phase_currents.b);
    if (fabsf(phase_currents.c - phc_current_calc) > 5)
    {
        App_GateDrive_Shutdown(gate_drive);
        App_CanTx_SetPeriodicSignal_MC_PHC_CUR_CALC_IMPLAUSIBLE(can_tx, 1);
    }

    dqs_currents = clarkeParkTransform(&phase_currents, rotor_position);

    bus_voltage = App_PowerStage_GetBusVoltage(power_stage);
    if (bus_voltage < MIN_BUS_VOLTAGE || bus_voltage > MAX_BUS_VOLTAGE)
    {
        App_CanTx_SetPeriodicSignal_MC_BUS_VOLTAGE_FAULT(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_MC_BUS_VOLTAGE_FAULT(can_tx, false);
    }

    if(state == CANMSGS_INV_STATE_STATE_DRIVE_CHOICE) {
        if (mode == GEN_SINE_M) {
            mod_index_ref = mod_index_ref > 0.9f ? 0.9f : mod_index_ref < 0 ? 0 : mod_index_ref;

            rotor_position = fmodf(
                    (prev_rotor_position +
                     (fund_freq_ref / SAMPLE_FREQUENCY) * 2 * (float)M_PI),
                    2 * (float)M_PI);

            dqs_voltages.q = mod_index_ref;
            dqs_voltages.d = 0;
            dqs_voltages.s = sqrtf(
                    dqs_voltages.q * dqs_voltages.q +
                    dqs_voltages.d * dqs_voltages.d);

            // Transform d/q voltages to phase voltages
            phase_voltages = parkClarkeTransform(&dqs_voltages, rotor_position);

            // Use Space Vector Modulation to calculate PWM durations.
            // Fake out bus voltage for Gen Sine M
            phase_duration = CalculatePwmEdges(&phase_voltages, (float)M_SQRT3);

            mod_index = dqs_voltages.s;

        } else if (mode == GEN_SINE_I) {
            ph_cur_peak_ref = App_CanRx_INV_PH_CUR_PEAK_REQ_GetSignal_PH_CUR_PEAK_REQ(can_rx);

            // Reset current controllers if
            if (prev_mode != mode) {
                iq_controller.integral_sum = 0;
                id_controller.integral_sum = 0;
                speed_controller.integral_sum = 0;
            }

            rotor_position = fmodf(
                    (prev_rotor_position +
                     (fund_freq_ref / SAMPLE_FREQUENCY) * 2 * (float)M_PI),
                    2 * (float)M_PI);

            dqs_ref_currents.q = ph_cur_peak_ref;
            dqs_ref_currents.d = 0;
            dqs_ref_currents.s = sqrtf(
                    dqs_ref_currents.q * dqs_ref_currents.q +
                    dqs_ref_currents.d * dqs_ref_currents.d);

            dqs_voltages = calculateDqsVoltages(
                    &dqs_ref_currents, &dqs_currents, rotor_speed / 9.549f,
                    bus_voltage, &id_controller, &iq_controller);

            id_controller.output = dqs_voltages.d;
            iq_controller.output = dqs_voltages.q;

            // Transform d/q voltages to phase voltages
            phase_voltages = parkClarkeTransform(&dqs_voltages, rotor_position);

            // Use Space Vector Modulation to calculate PWM durations
            phase_duration = CalculatePwmEdges(&phase_voltages, bus_voltage);

            // Calculate modulation index
            if (bus_voltage == 0.0f)
            {
                mod_index = dqs_voltages.s /
                            ((bus_voltage + 0.000001f) / (float)M_SQRT3);
            }
            else
            {
                mod_index = dqs_voltages.s / ((bus_voltage) / (float)M_SQRT3);
            }

        } else if (mode == MOTOR_CONTROL) {

            // Reset current controllers if changing modes
            if (prev_mode != mode) {
                iq_controller.integral_sum = 0;
                id_controller.integral_sum = 0;
                speed_controller.integral_sum = 0;
            }

            //Calculate rotor speed
            float rotor_position_diff = rotor_position - prev_rotor_position;

            //Rotor position difference exceeds max diff (rolling over from 2pi to 0)
            if (fabsf(rotor_position_diff) > MAX_MOTOR_POS_CHANGE_PER_CYCLE) {
                // Rotor is moving in the forward direction, and crossing
                // 360degrees
                if (prev_rotor_position > rotor_position) {
                    rotor_speed = (rotor_position - (2 * (float) M_PI - prev_rotor_position)) * SAMPLE_FREQUENCY;
                }
                    // Rotor is moving in the reverse direction, and crossing
                    // 360degrees
                else if (prev_rotor_position < rotor_position) {
                    rotor_speed = ((rotor_position - 2 * (float) M_PI) - prev_rotor_position) * SAMPLE_FREQUENCY;
                }
            } else {
                rotor_speed = (rotor_position - prev_rotor_position) * SAMPLE_FREQUENCY;
            }

            // Rotor speed/position are implausible
            if (
                    fabsf(rotor_position_diff) > MAX_MOTOR_POS_CHANGE_PER_CYCLE &&
                    fabsf(rotor_position_diff) <
                    2 * (float) M_PI - MAX_MOTOR_POS_CHANGE_PER_CYCLE) {
                App_GateDrive_Shutdown(gate_drive);
                App_CanTx_SetPeriodicSignal_MC_ROTOR_POSITION_IMPLAUSIBLE(
                        can_tx, 1);
            }

            if(mc_mode == SPEED_CONTROL)
            {
                // calculate torque output of speed PI controller
                calculatePiOutputs(&speed_controller, rotor_speed_ref, rotor_speed, MAX_MOTOR_TORQUE, 0);
            }
            else
            {
                if(!LUT_CONTROL_ENABLED) {
                    //Overwrite torque_ref from torque control if speed control is active
                    dqs_ref_currents.s = torqueControl(
                            rotor_speed_ref, rotor_speed, torque_ref, &speed_controller,
                            prev_fw_flag);
                }
            }

            //If used, calculate adaption gains
            if (ADAPTION_GAIN_ENABLED)
            {
                id_controller =
                        adaptionGain(&id_controller, dqs_ref_currents.s);
                iq_controller =
                        adaptionGain(&iq_controller, dqs_ref_currents.s);
            }
            if (LUT_CONTROL_ENABLED) {
                // LUT may look up some nonzero number for 0 torque, so set to zero
                if (torque_ref == 0.0f) {
                    dqs_ref_currents.d = 0.0f;
                    dqs_ref_currents.q = 0.0f;
                } else {
                    look_up_dqs_current_ref(
                            rotor_speed, torque_ref, bus_voltage,
                            stator_current_limit, &dqs_ref_currents);
                }
            }
            else
            {
                dqs_ref_currents = generateRefCurrents(
                        &dqs_ref_currents, rotor_speed, bus_voltage, &fw_flag);
            }

            dqs_voltages = calculateDqsVoltages(
                    &dqs_ref_currents, &dqs_currents, rotor_speed / 9.549f,
                    bus_voltage, &id_controller, &iq_controller);

            id_controller.output = dqs_voltages.d;
            iq_controller.output = dqs_voltages.q;

            // Transform d/q voltages to phase voltages
            phase_voltages = parkClarkeTransform(&dqs_voltages, rotor_position);

            // Use Space Vector Modulation to calculate PWM durations
            phase_duration = CalculatePwmEdges(&phase_voltages, bus_voltage);

            // Calculate modulation index
            if (bus_voltage == 0.0f)
            {
                mod_index = dqs_voltages.s /
                            ((bus_voltage + 0.000001f) / (float)M_SQRT3);
            }
            else
            {
                mod_index = dqs_voltages.s / ((bus_voltage) / (float)M_SQRT3);
            }
        }

        //For all modes, load PWM signals
        App_GateDrive_LoadPwm(gate_drive, &phase_duration);

        prev_fw_flag        = fw_flag;
        prev_rotor_position = rotor_position;
        prev_rotor_speed    = rotor_speed;
        prev_mode           = mode;
        prev_mc_mode        = mc_mode;

    }
    //Not in the drive state
    else
    {
        // Apply safe ASC state if inverter is not in the drive state
        phase_duration.a = 1.0f;
        phase_duration.b = 1.0f;
        phase_duration.c = 1.0f;
        App_GateDrive_LoadPwm(gate_drive, &phase_duration);

        App_CanTx_SetPeriodicSignal_MODE(
                can_tx, CANMSGS_INV_STATE_MODE_UNDEFINED_CHOICE);
    }
}

void App_ControlLoop_GetIqControllerValues(struct ControllerValues *controller)
{
    *controller = iq_controller;
}

void App_ControlLoop_GetIdControllerValues(struct ControllerValues *controller)
{
    *controller = id_controller;
}

float App_ControlLoop_GetLineToNeutralVoltage(void)
{
    return dqs_voltages.s;
}

void App_ControlLoop_GetSpeedControllerValues(
    struct ControllerValues *controller)
{
    *controller = speed_controller;
}

uint8_t App_ControlLoop_GetMode(void)
{
    return mode;
}

float App_ControlLoop_GetModIndex(void)
{
    return mod_index;
}

float App_ControlLoop_GetRotorPosition(void)
{
    return rotor_position*360.0f/(2*(float)M_PI);
}

float App_ControlLoop_GetRotorSpeed(void)
{
    return rotor_speed;
}

float App_ControlLoop_GetPhcCurCalc(void)
{
    return phc_current_calc;
}

bool App_ControlLoop_GetFwFlag(void)
{
    return fw_flag;
}
void App_ControlLoop_GetDqsRefCurrents(struct DqsValues *dqs_values)
{
    *dqs_values = dqs_ref_currents;
}

void App_ControlLoop_GetDqsActualCurrents(struct DqsValues *dqs_values)
{
    *dqs_values = dqs_currents;
}

float App_ControlLoop_GetPowerStageCurrentLimit(void)
{
    return powerstage_derated_current_limit;
}

float App_ControlLoop_GetMotorCurrentLimit(void)
{
    return motor_derated_current_limit;
}

float App_ControlLoop_GetCurrentLimit(void)
{
    return stator_current_limit;
}
