#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(DcmCanTxInterface)

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
