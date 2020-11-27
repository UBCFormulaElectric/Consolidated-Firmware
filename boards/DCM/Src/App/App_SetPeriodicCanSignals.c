#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"
#include "configs/App_TorqueRequestThresholds.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(DcmCanTxInterface)

// TODO: Implement PID controller to maintain DC bus power at 80kW
// #680
void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world)
{
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    // Regen allowed when braking or (speed > 5kmh and AIRs closed)
    const float regen_threshold_kph = 5.0f;
    const bool  is_every_air_closed =
        (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx) ==
         CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE) &&
        (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx) ==
         CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    const bool is_vehicle_over_regen_threshold =
        (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_LEFT_WHEEL_SPEED(can_rx) >
         regen_threshold_kph) &&
        (App_CanRx_FSM_WHEEL_SPEED_SENSOR_GetSignal_RIGHT_WHEEL_SPEED(can_rx) >
         regen_threshold_kph);
    const bool is_regen_allowed =
        is_vehicle_over_regen_threshold && is_every_air_closed;

    const float regen_paddle_percentage =
        (float)App_CanRx_DIM_REGEN_PADDLE_GetSignal_MAPPED_PADDLE_POSITION(
            can_rx);
    float torque_request;

    //     Calculating the torque request
    //
    //      1) If regen is on, use the regen paddle percentage
    //      2) If regen. braking is off, use the accelerator pedal percentage
    //
    //      - MAX_SAFE_TORQUE_REQUEST_NM = 21 Nm, the max torque the motor can
    //      provide
    //
    //      1) If regen. braking is allowed and the regen paddle is actuated,
    //         the torque request (in Nm) is negative and given by:
    //
    //                                (-) Regen Paddle Percentage
    //        Torque Request    =   -------------------------------  *
    //        MAX_SAFE_TORQUE_REQUEST_NM
    //                                            100%
    //      2) Otherwise, the torque request (in Nm) is positive and given by:
    //
    //                                      Accelerator Pedal Percentage
    //        Torque Request    =         -------------------------------  *
    //        MAX_SAFE_TORQUE_REQUEST_NM
    //                                               100%

    if (regen_paddle_percentage > 0.0f && is_regen_allowed)
    {
        torque_request =
            -0.01f * regen_paddle_percentage * MAX_SAFE_TORQUE_REQUEST_NM;
    }
    else
    {
        torque_request =
            0.01f *
            App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(
                can_rx) *
            MAX_SAFE_TORQUE_REQUEST_NM;
    }

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx, torque_request);
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
