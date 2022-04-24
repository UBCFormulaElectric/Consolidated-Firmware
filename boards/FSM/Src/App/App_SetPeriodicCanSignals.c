#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedTorqueRequest.h"

// For torque plausibility checking between FSM and DCM
#define MAX_TORQUE_PLAUSIBILITY_ERROR_COUNT (3U)
#define TORQUE_REQUEST_UPPER_BOUND_OFFSET_NM (5.0f)

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

void App_SetPeriodicSignals_FlowRateInRangeChecks(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *primary_flow_rate_in_range_check =
        App_FsmWorld_GetPrimaryFlowRateInRangeCheck(world);
    struct InRangeCheck *secondary_flow_rate_in_range_check =
        App_FsmWorld_GetSecondaryFlowRateInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, primary_flow_rate_in_range_check,
        App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, secondary_flow_rate_in_range_check,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_WheelSpeedInRangeChecks(
    const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *left_wheel_speed_in_range_check =
        App_FsmWorld_GetLeftWheelSpeedInRangeCheck(world);
    struct InRangeCheck *right_wheel_speed_in_range_check =
        App_FsmWorld_GetRightWheelSpeedInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, left_wheel_speed_in_range_check,
        App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED,
        App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_wheel_speed_in_range_check,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_SteeringAngleInRangeCheck(
    const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *steering_angle_in_range_check =
        App_FsmWorld_GetSteeringAngleInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, steering_angle_in_range_check,
        App_CanTx_SetPeriodicSignal_STEERING_ANGLE,
        App_CanTx_SetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_Brake(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct Brake *brake = App_FsmWorld_GetBrake(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Brake_GetPressureInRangeCheck(brake),
        App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE,
        App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    if (App_Brake_IsBrakeActuated(brake))
    {
        App_CanTx_SetPeriodicSignal_BRAKE_IS_ACTUATED(
            can_tx, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BRAKE_IS_ACTUATED(
            can_tx, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_FALSE_CHOICE);
    }

    if (App_Brake_IsPressureSensorOpenOrShortCircuit(brake))
    {
        App_CanTx_SetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT(
            can_tx,
            CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT(
            can_tx,
            CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE);
    }
}

void App_SetPeriodicSignals_AcceleratorPedal(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct Brake *            brake = App_FsmWorld_GetBrake(world);
    struct AcceleratorPedals *papps_and_sapps =
        App_FsmWorld_GetPappsAndSapps(world);

    const float papps_pedal_percentage =
        App_AcceleratorPedals_GetPrimaryPedalPercentage(papps_and_sapps);
    const float sapps_pedal_percentage =
        App_AcceleratorPedals_GetSecondaryPedalPercentage(papps_and_sapps);

    App_CanTx_SetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(
        can_tx, papps_pedal_percentage);
    App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(
        can_tx, sapps_pedal_percentage);

    if (App_Brake_IsBrakeActuated(brake))
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(
            can_tx, papps_pedal_percentage);
    }
}

void App_SetPeriodicSignals_MotorShutdownFaults(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);
    struct FsmCanRxInterface *can_rx = App_FsmWorld_GetCanRx(world);
    static uint8_t            torque_plausibility_err_cnt  = 0U;
    static float              prev_expected_torque_request = 0U;

    // Perform torque plausibility check if DCM is in the drive state
    if (App_CanRx_DCM_STATE_MACHINE_GetSignal_STATE(can_rx) ==
        CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE)
    {
        // Compare the current DCM torque request to the calculated torque
        // request on the previous invocation to account for delay
        const float upper_torque_request_bound =
            prev_expected_torque_request + TORQUE_REQUEST_UPPER_BOUND_OFFSET_NM;
        const float dcm_torque_request_invl =
            App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
                App_CanRx_DCM_INVL_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVL(
                    can_rx));
        const float dcm_torque_request_invr =
            App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
                App_CanRx_DCM_INVR_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVR(
                    can_rx));

        if (dcm_torque_request_invl > upper_torque_request_bound ||
            dcm_torque_request_invr > upper_torque_request_bound)
        {
            torque_plausibility_err_cnt++;
        }
        else
        {
            torque_plausibility_err_cnt = 0U;
        }

        if (torque_plausibility_err_cnt == MAX_TORQUE_PLAUSIBILITY_ERROR_COUNT)
        {
            App_CanTx_SetPeriodicSignal_TORQUE_PLAUSIBILITY_CHECK_FAILED(
                can_tx,
                CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_TORQUE_PLAUSIBILITY_CHECK_FAILED_TRUE_CHOICE);
        }

        prev_expected_torque_request =
            App_SharedTorqueRequest_CalculateTorqueRequest(
                App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED(can_tx),
                App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED(can_tx),
                App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx),
                App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx),
                (float)
                    App_CanRx_DIM_REGEN_PADDLE_GetSignal_MAPPED_PADDLE_POSITION(
                        can_rx),
                App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx));
    }

    App_CanTx_SetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE);
    App_CanTx_SetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE);
    App_CanTx_SetPeriodicSignal_APPS_HAS_DISAGREEMENT(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
    App_CanTx_SetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE);
    App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE);
    App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(
        can_tx,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE);
}
