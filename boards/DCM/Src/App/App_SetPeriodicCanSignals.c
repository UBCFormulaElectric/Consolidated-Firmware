#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"
#include "App_SharedTorqueRequest.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(DcmCanTxInterface)

// TODO: Implement PID controller to maintain DC bus power at 80kW
// #680
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world)
{
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    const float torque_request = App_SharedTorqueRequest_CalculateTorqueRequest(
        App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_RIGHT_WHEEL_SPEED(can_rx),
        App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_LEFT_WHEEL_SPEED(can_rx),
        App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx),
        App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx),
        (float)App_CanRx_DIM_REGEN_PADDLE_GetSignal_MAPPED_PADDLE_POSITION(
            can_rx),
        App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx));

    // Transmit torque command to both inverters
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(
                    torque_request));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(
                    torque_request));
}

void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world)
{
    struct Imu *              imu    = App_DcmWorld_GetImu(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationXInRangeCheck(imu),
        App_CanTx_SetPeriodicSignal_ACCELERATION_X,
        App_CanTx_SetPeriodicSignal_ACCELERATION_X_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_X_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationYInRangeCheck(imu),
        App_CanTx_SetPeriodicSignal_ACCELERATION_Y,
        App_CanTx_SetPeriodicSignal_ACCELERATION_Y_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Y_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Imu_GetAccelerationZInRangeCheck(imu),
        App_CanTx_SetPeriodicSignal_ACCELERATION_Z,
        App_CanTx_SetPeriodicSignal_ACCELERATION_Z_OUT_OF_RANGE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_ACCELERATION_Z_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
