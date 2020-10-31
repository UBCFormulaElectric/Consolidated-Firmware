#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(DcmCanTxInterface)

// TODO: Implement PID controller to maintain DC bus power at 80kW
// #680
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world)
{
    float MAX_SAFE_TORQUE_REQUEST =
        21.0f; // 21Nm is max torque each motor can handle

    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    float pedal_percentage =
        App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx);

    float regen_paddle_percentage =
        (float)App_CanRx_DIM_REGEN_PADDLE_GetSignal_MAPPED_PADDLE_POSITION(
            can_rx);
    float torque_request;

    // Regen allowed when braking or (speed > 5kmh and AIRs closed)
    float threshold_speed = 5.0f; // 5kmh
    bool  air_closed =
        (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx) ==
         CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE) &&
        (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx) ==
         CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    bool going_fast_enough =
        (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_LEFT_WHEEL_SPEED(can_rx) >
         threshold_speed) &&
        (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_RIGHT_WHEEL_SPEED(can_rx) >
         threshold_speed);
    bool regen_allowed = going_fast_enough && air_closed;

    if (regen_paddle_percentage > 0 && regen_allowed)
    {
        torque_request =
            -0.01f * regen_paddle_percentage * MAX_SAFE_TORQUE_REQUEST;
    }
    else
    {
        torque_request = 0.01f * pedal_percentage * MAX_SAFE_TORQUE_REQUEST;
    }

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx, torque_request);
}

void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world)
{
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);
    struct Imu *              imu    = App_DcmWorld_GetImu(world);

    ExitCode data_valid = App_Imu_UpdateSensorData(imu);

    if (data_valid != EXIT_CODE_OK)
        return;

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
