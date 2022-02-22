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
#include "lookup_tables/motor_lut_interface.h"
//#include "states/App_AllStates.h"
#include "App_PowerStage.h"
#include "App_Motor.h"

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

static struct MotorControlFaults motor_control_faults = {
    .torque_request_implausible = 0,
    .rotor_position_implausible = 0,
    .phc_current_implausible    = 0,
    .fund_freq_req_implausible  = 0
};

static float   rotor_position      = 0;
static float   prev_rotor_position = 0;
static float   rotor_speed         = 0;
static float   prev_rotor_speed    = 0;
static float   bus_voltage         = 0;
static float   mod_index           = 0;
static float   phc_current_calc    = 0;
static float   torque_ref          = 0;
static bool    fw_flag             = 0;
static bool    prev_fw_flag;
static uint8_t mode            = MODE_UNDEFINED;
static float   rotor_speed_ref = 0;
static float   mod_index_ref   = 0;
static float   ph_cur_peak_ref = 0;
static float   fund_freq_ref   = 0;

// TODO parameters passed in here should come from the CANbus. this is just for
// debugging. It should be void
void App_ControlLoop_Run(const struct InvWorld *world)
{
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);
    struct InvCanRxInterface *can_rx      = App_InvWorld_GetCanRx(world);

    // Get user requests from the CAN bus
    rotor_speed_ref =
        App_CanRx_INV_ROTOR_SPEED_REQ_GetSignal_ROTOR_SPEED_REQ(can_rx);
    mode          = App_CanRx_INV_MODE_REQ_GetSignal_MODE_REQ(can_rx);
    mod_index_ref = App_CanRx_INV_MOD_INDEX_REQ_GetSignal_MOD_INDEX_REQ(can_rx);
    ph_cur_peak_ref =
        App_CanRx_INV_PH_CUR_PEAK_REQ_GetSignal_PH_CUR_PEAK_REQ(can_rx);

    if (mode == MOTOR_CONTROL)
    {
        torque_ref =
            App_CanRx_DCM_TORQUE_REQUEST_GetSignal_TORQUE_REQUEST(can_rx);
        if (fabsf(torque_ref) > MAX_MOTOR_TORQUE)
        {
            motor_control_faults.torque_request_implausible = 1;
            torque_ref                                      = 0;
        }
        rotor_position            = App_Motor_GetPositionBlocking();
        float rotor_position_diff = rotor_position - prev_rotor_position;
        if (fabsf(rotor_position_diff) > MAX_MOTOR_POS_CHANGE_PER_CYCLE)
        {
            // Rotor is moving in the positive direction, and crossing
            // 360degrees
            if (prev_rotor_position > rotor_position)
            {
                rotor_speed =
                    (rotor_position - prev_rotor_position - 2 * (float)M_PI) *
                    SAMPLE_FREQUENCY;
            }
            // Rotor is moving in the negative direction, and crossing
            // 360degrees
            else if (rotor_position < prev_rotor_position)
            {
                rotor_speed =
                    (prev_rotor_position - rotor_position - 2 * (float)M_PI) *
                    SAMPLE_FREQUENCY;
            }
        }
        else if (
            fabsf(rotor_position_diff) > MAX_MOTOR_POS_CHANGE_PER_CYCLE &&
            fabsf(rotor_position_diff) <
                2 * (float)M_PI - MAX_MOTOR_POS_CHANGE_PER_CYCLE)
        {
            // Rotor speed/position are implausible
            motor_control_faults.rotor_position_implausible = 1;
            rotor_speed                                     = 0;
            rotor_position                                  = 0;
        }
        else
        {
            rotor_speed =
                (rotor_position - prev_rotor_position) * SAMPLE_FREQUENCY;
        }
    }
    else
    {
        torque_ref = 0;
    }

    // Get Phase Currents
    App_PowerStage_GetPhaseCurrents(power_stage, &phase_currents);
    phc_current_calc = -1 * (phase_currents.a + phase_currents.b);
    if (fabsf(phase_currents.c - phc_current_calc) > 5)
    {
        motor_control_faults.phc_current_implausible = 1;
    }

    // Fake out rotor position for sinewave generation modes
    if (mode == GEN_SINE_I || mode == GEN_SINE_M)
    {
        fund_freq_ref =
            App_CanRx_INV_FUND_FREQ_REQ_GetSignal_FUND_FREQ_REQ(can_rx);
        if (fund_freq_ref < 0 ||
            fund_freq_ref > MAX_MOTOR_SPEED / 60.0f * (MOTOR_POLES / 2.0f))
        {
            motor_control_faults.fund_freq_req_implausible = 1;
            fund_freq_ref = 100.0f; // safe default fundamental frequency
        }
        rotor_position = fmodf(
            (prev_rotor_position +
             (fund_freq_ref / SAMPLE_FREQUENCY) * 2 * (float)M_PI),
            2 * (float)M_PI);
    }

    bus_voltage = App_PowerStage_GetBusVoltage(power_stage);

    dqs_currents = clarkeParkTransform(&phase_currents, rotor_position);

    // Get stator current reference
    if (mode == MOTOR_CONTROL)
    {
        dqs_ref_currents.s = torqueControl(
            rotor_speed_ref, rotor_speed, torque_ref, &speed_controller,
            prev_fw_flag);

        //    // If used, calculate adaption gains
        if (ADAPTION_GAIN_ENABLED)
        {
            id_controller = adaptionGain(&id_controller, dqs_ref_currents.s);
            iq_controller = adaptionGain(&iq_controller, dqs_ref_currents.s);
        }
    }

    if (mode == GEN_SINE_I)
    {
        dqs_ref_currents.q = ph_cur_peak_ref;
        dqs_ref_currents.d = 0;
        dqs_ref_currents.s = sqrtf(
            dqs_ref_currents.q * dqs_ref_currents.q +
            dqs_ref_currents.d * dqs_ref_currents.d);
    }
    if (mode == MOTOR_CONTROL)
    {
        if (LUT_CONTROL_ENABLED)
        {
            dqs_ref_currents.d = look_up_value(
                rotor_speed, torque_ref, bus_voltage, 80.0f, ID_PEAK);
            dqs_ref_currents.q = look_up_value(
                rotor_speed, torque_ref, bus_voltage, 80.0f, IQ_PEAK);
            // fw_flag =
        }
        else
        {
            dqs_ref_currents = generateRefCurrents(
                &dqs_ref_currents, rotor_speed, bus_voltage, &fw_flag);
        }
    }

    // Calculate d/q PI controller outputs
    if (mode == GEN_SINE_M)
    {
        dqs_voltages.q = mod_index_ref * (0.9f * bus_voltage / (float)M_SQRT3);
        dqs_voltages.d = 0;
        dqs_voltages.s = sqrtf(
            dqs_voltages.q * dqs_voltages.q + dqs_voltages.d * dqs_voltages.d);
    }
    else
    {
        dqs_voltages = calculateDqsVoltages(
            &dqs_ref_currents, &dqs_currents, rotor_speed, bus_voltage,
            &id_controller, &iq_controller);
    }

    id_controller.output = dqs_voltages.d;
    iq_controller.output = dqs_voltages.q;
    mod_index            = dqs_voltages.s / (float)M_SQRT3;

    // Transform d/q voltages to phase voltages
    phase_voltages = parkClarkeTransform(&dqs_voltages, rotor_position);

    // Use Space Vector Modulation to calculate PWM durations
    phase_duration = CalculatePwmEdges(&phase_voltages, bus_voltage);

    App_GateDrive_LoadPwm(gate_drive, &phase_duration);

    prev_fw_flag        = fw_flag;
    prev_rotor_position = rotor_position;
    prev_rotor_speed    = rotor_speed;
}

void App_ControlLoop_GetIqControllerValues(struct ControllerValues *controller)
{
    *controller = iq_controller;
}

void App_ControlLoop_GetIdControllerValues(struct ControllerValues *controller)
{
    *controller = id_controller;
}

void App_ControlLoop_GetSpeedControllerValues(
    struct ControllerValues *controller)
{
    *controller = speed_controller;
}

void App_ControlLoop_GetFaults(struct MotorControlFaults *faults)
{
    *faults = motor_control_faults;
}

uint8_t App_ControlLoop_GetMode(void)
{
    return mode;
}

float App_ControlLoop_GetModIndex(void)
{
    return mod_index;
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
