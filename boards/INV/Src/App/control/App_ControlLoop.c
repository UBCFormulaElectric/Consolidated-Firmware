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
struct PhaseValues phase_voltages, phase_currents, phase_duration;
struct DqsValues   dqs_ref_currents, dqs_currents, dqs_voltages;

struct ControllerValues speed_controller = {
    .prev_integral_input = 0,
    .integral_sum        = 0,
    .gain                = SPEED_GAIN,
    .time_const          = SPEED_TIME_CONST,
};

struct ControllerValues id_controller = { .prev_integral_input = 0,
                                          .integral_sum        = 0,
                                          .gain                = D_GAIN,
                                          .time_const          = D_TIME_CONST };

struct ControllerValues iq_controller = { .prev_integral_input = 0,
                                          .integral_sum        = 0,
                                          .gain                = Q_GAIN,
                                          .time_const          = Q_TIME_CONST };

static float rotor_position      = 0;
static float prev_rotor_position = 0;
static float rotor_speed         = 0;
static float bus_voltage         = 0;
static float phc_current_calc    = 0;
static float torque_ref          = 0;
static bool  fw_flag             = 0;
static bool  prev_fw_flag;
static uint8_t mode = MODE_UNDEFINED;
static float rotor_speed_ref = 0;
static float mod_index_ref = 0;
static float ph_cur_rms_ref = 0;

//TODO parameters passed in here should come from the CANbus. this is just for debugging. It should be void
void App_ControlLoop_Run(
    const float            rotor_speed_ref_request,
    const uint8_t          mode_request,
    const struct InvWorld *world,
    const float            mod_index_ref_request,
    float                  ph_cur_rms_ref_request)
{
    rotor_speed_ref = rotor_speed_ref_request;
    mode = mode_request;
    mod_index_ref = mod_index_ref_request;
    ph_cur_rms_ref = ph_cur_rms_ref_request;

    struct GateDrive * gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);
    //     struct InvMotor* motor = App_InvWorld_GetMotor(world);
    struct InvCanRxInterface *can_rx = App_InvWorld_GetCanRx(world);

    if (mode_request == MOTOR_CONTROL)
    {
        App_CanRx_DCM_TORQUE_REQUEST_GetSignal_TORQUE_REQUEST(
            can_rx);
        rotor_position = App_Motor_GetPositionBlocking();
    }

    // TODO torque and speed request and reading plausibility check goes here
    torque_ref   = 15;
    float id_val = look_up_value(-40000.0f, -40.0f, 300.0f, 80.0f, ID_PEAK);
    float iq_val = look_up_value(22000.0f, 30.0f, 700.0f, 80.0f, IQ_PEAK);

    // TODO Get Speed Ref Request from state machine

    // Get Phase Currents
    App_PowerStage_GetPhaseCurrents(power_stage, &phase_currents);
    phc_current_calc = -1 * (phase_currents.a + phase_currents.b);
    if (fabsf(phase_currents.c - phc_current_calc) > 5)
    {
        // TODO Phase C Current calculation plausibility error
    }

    //    // Get Rotor Position
    if (mode_request == GEN_SINE_I || mode_request == GEN_SINE_M)
    {
        float fund_freq_request = rotor_speed_ref_request * (float)0.15915494327;
        rotor_position          = fmodf(
            (prev_rotor_position +
             (fund_freq_request / SAMPLE_FREQUENCY) * 2 * (float)M_PI),
            2 * (float)M_PI);
    }
    else
    {
        // rotor_position = (float)App_Motor_GetPosition();
        // rotor_position = App_Motor_Get
    }

    // Get Bus Voltage
    // bus_voltage = App_PowerStage_GetBusVoltage(power_stage);
    bus_voltage = 24.0f;

    if (mode_request == MOTOR_CONTROL)
    {
        // Calculate Rotor Speed
        // TODO bug here when rotor goes from 360 degrees to 0 degrees
        rotor_speed = (rotor_position - prev_rotor_position) * SAMPLE_FREQUENCY;
    }

    dqs_currents = clarkeParkTransform(&phase_currents, rotor_position);

    // Get stator current reference

    if (mode_request == MOTOR_CONTROL)
    {
        dqs_ref_currents.s = torqueControl(
            rotor_speed_ref_request, rotor_speed, torque_ref, &speed_controller,
            prev_fw_flag);

        //    // If used, calculate adaption gains
        if (ADAPTION_GAIN_ENABLED)
        {
            id_controller = adaptionGain(&id_controller, dqs_ref_currents.s);
            iq_controller = adaptionGain(&iq_controller, dqs_ref_currents.s);
        }
    }

    if (mode_request == GEN_SINE_I || mode_request == MOTOR_CONTROL)
    {
        //     Calculate d/q current references
        //     If GEN_SINE_I mode is used, current request is defined by the
        //     user.
        if (mode_request == GEN_SINE_I)
        {
            dqs_ref_currents.q = ph_cur_rms_ref;
            dqs_ref_currents.d = 0;
            dqs_ref_currents.s = sqrtf(
                dqs_ref_currents.q * dqs_ref_currents.q +
                dqs_ref_currents.d * dqs_ref_currents.d);
        }
        if (mode_request == MOTOR_CONTROL)
        {
            dqs_ref_currents = generateRefCurrents(
                &dqs_ref_currents, rotor_speed, bus_voltage, &fw_flag);
        }
    }

    // Calculate d/q PI controller outputs
    if (mode_request == GEN_SINE_M)
    {
        dqs_voltages.q =
            mod_index_ref * (0.9f * bus_voltage / (float)M_SQRT3);
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

    // Transform d/q voltages to phase voltages
    phase_voltages = parkClarkeTransform(&dqs_voltages, rotor_position);

    // Use Space Vector Modulation to calculate PWM durations
    phase_duration = CalculatePwmEdges(&phase_voltages, bus_voltage);

    App_GateDrive_LoadPwm(gate_drive, &phase_duration);

    prev_fw_flag        = fw_flag;
    prev_rotor_position = rotor_position;
}

void App_ControlLoop_GetIqControllerValues(struct ControllerValues* controller)
{
    controller = &iq_controller;
}

void App_ControlLoop_GetIdControllerValues(struct ControllerValues* controller)
{
    controller = &id_controller;
}

void App_ControlLoop_GetSpeedControllerValues(struct ControllerValues* controller)
{
    controller = &speed_controller;
}

uint8_t App_ControlLoop_GetMode(void)
{
    return mode;
}

