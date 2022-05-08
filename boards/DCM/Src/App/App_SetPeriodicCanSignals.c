#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"
#include "states/App_SharedStates.h"
#include "configs/App_TorqueRequestThresholds.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(DcmCanTxInterface)

/**
 * Check if both AIRs are closed
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if both AIRs are closed, false otherwise
 */
static inline bool App_AreBothAIRsClosed(const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx_interface) ==
               CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE &&
           App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx_interface) ==
               CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE;
}

/**
 * Check if vehicle is over the regen threshold defined by vehicle wheel speed
 * (EV.4.1.3)
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the vehicle is over the regen threshold, false otherwise
 */
static inline bool App_IsVehicleOverRegenThresh(const struct DcmCanRxInterface *can_rx_interface)
{
    return (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_LEFT_WHEEL_SPEED(can_rx_interface) >
            REGEN_WHEEL_SPEED_THRESHOLD_KPH) &&
           (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_RIGHT_WHEEL_SPEED(can_rx_interface) >
            REGEN_WHEEL_SPEED_THRESHOLD_KPH);
}

// TODO: Implement PID controller to maintain DC bus power at 80kW
// #680
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world)
{
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    // Regen allowed when wheel speed > REGEN_WHEEL_SPEED_THRESHOLD_KPH and AIRs
    // closed
    const bool is_regen_allowed = App_AreBothAIRsClosed(can_rx) && App_IsVehicleOverRegenThresh(can_rx);

    const float regen_paddle_percentage = (float)App_CanRx_DIM_REGEN_PADDLE_GetSignal_MAPPED_PADDLE_POSITION(can_rx);
    float       torque_request;

    // Calculating the torque request
    // 1) If regen is allowed and the regen paddle is actuated, use the regen
    // paddle percentage 2) If regen is not allowed, use the accelerator pedal
    // percentage
    //
    // Constants:  MAX_TORQUE_REQUEST_NM = 132 Nm,
    //              - the max torque the motor can provide
    //
    // 1) If regen is allowed and the regen paddle is actuated,
    //         the torque request (in Nm) is negative and given by:
    //              (-) Regen Paddle Percentage
    //  Torque  =  -------------------------------  * MAX_TORQUE_REQUEST_NM
    //                         100%
    // 2) Otherwise, the torque request (in Nm) is positive and given by:
    //              Accelerator Pedal Percentage
    //  Torque =  -------------------------------  * MAX_TORQUE_REQUEST_NM
    //                        100%

    if (regen_paddle_percentage > 0.0f && is_regen_allowed)
    {
        torque_request = -0.01f * regen_paddle_percentage * MAX_TORQUE_REQUEST_NM;
    }
    else
    {
        torque_request =
            0.01f * App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx) * MAX_TORQUE_REQUEST_NM;
    }

    // Transmit torque command to both inverters
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(torque_request));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(torque_request));
}

void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world)
{
    struct Imu *              imu    = App_DcmWorld_GetImu(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationXInRangeCheck(imu), App_CanTx_SetPeriodicSignal_ACCELERATION_X,
        App_CanTx_SetPeriodicSignal_ACCELERATION_X_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationYInRangeCheck(imu), App_CanTx_SetPeriodicSignal_ACCELERATION_Y,
        App_CanTx_SetPeriodicSignal_ACCELERATION_Y_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationZInRangeCheck(imu), App_CanTx_SetPeriodicSignal_ACCELERATION_Z,
        App_CanTx_SetPeriodicSignal_ACCELERATION_Z_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
