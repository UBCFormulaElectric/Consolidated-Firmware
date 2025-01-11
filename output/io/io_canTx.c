/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canTx.h"
#include <string.h>
#include "app_canTx.h"
#include "app_canUtils.h"

#ifndef THREAD_SAFE_CAN_PACKING
void (*lock)(void);
void (*unlock)(void); 

void io_canTx_set_lockfunction(
    void (*lock)(void),
    void (*unlock)(void)
)
{
    lock = lock;
    unlock = unlock;
}
#endif

/* --------------------------- Static Variables --------------------------- */

static uint32_t can_mode_bus1;
static void (*transmit_func_bus1)(const JsonCanMsg* tx_msg);

static uint32_t can_mode_bus2;
static void (*transmit_func_bus2)(const JsonCanMsg* tx_msg);


/* --------------------- Static Function Definitions ---------------------- */

static void io_canTx_VC_Vitals_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_VITALS_ID;
    tx_msg.dlc = CANMSG_VC_VITALS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_Vitals_pack(app_canTx_VC_Vitals_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_LeftInverterCommand_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_LEFT_INVERTER_COMMAND_ID;
    tx_msg.dlc = CANMSG_VC_LEFT_INVERTER_COMMAND_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_LeftInverterCommand_pack(app_canTx_VC_LeftInverterCommand_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
staic void io_canTx_VC_INVL_ReadWriteParamCommand_sendAperiodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_INVL_READ_WRITE_PARAM_COMMAND_ID;
    tx_msg.dlc = CANMSG_VC_INVL_READ_WRITE_PARAM_COMMAND_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_INVL_ReadWriteParamCommand_pack(app_canTx_VC_INVL_ReadWriteParamCommand_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_RightInverterCommand_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_RIGHT_INVERTER_COMMAND_ID;
    tx_msg.dlc = CANMSG_VC_RIGHT_INVERTER_COMMAND_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_RightInverterCommand_pack(app_canTx_VC_RightInverterCommand_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
staic void io_canTx_VC_INVR_ReadWriteParamCommand_sendAperiodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_INVR_READ_WRITE_PARAM_COMMAND_ID;
    tx_msg.dlc = CANMSG_VC_INVR_READ_WRITE_PARAM_COMMAND_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_INVR_ReadWriteParamCommand_pack(app_canTx_VC_INVR_ReadWriteParamCommand_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_AlertsContext_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ALERTS_CONTEXT_ID;
    tx_msg.dlc = CANMSG_VC_ALERTS_CONTEXT_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_AlertsContext_pack(app_canTx_VC_AlertsContext_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_EllipseStatus_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ELLIPSE_STATUS_ID;
    tx_msg.dlc = CANMSG_VC_ELLIPSE_STATUS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_EllipseStatus_pack(app_canTx_VC_EllipseStatus_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_EllipseTime_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ELLIPSE_TIME_ID;
    tx_msg.dlc = CANMSG_VC_ELLIPSE_TIME_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_EllipseTime_pack(app_canTx_VC_EllipseTime_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_EllipseEulerAngles_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ELLIPSE_EULER_ANGLES_ID;
    tx_msg.dlc = CANMSG_VC_ELLIPSE_EULER_ANGLES_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_EllipseEulerAngles_pack(app_canTx_VC_EllipseEulerAngles_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_CommitInfo_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_COMMIT_INFO_ID;
    tx_msg.dlc = CANMSG_VC_COMMIT_INFO_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_CommitInfo_pack(app_canTx_VC_CommitInfo_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_PowerEstimate_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_POWER_ESTIMATE_ID;
    tx_msg.dlc = CANMSG_VC_POWER_ESTIMATE_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_PowerEstimate_pack(app_canTx_VC_PowerEstimate_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_PowerEstimatePID_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_POWER_ESTIMATE_PID_ID;
    tx_msg.dlc = CANMSG_VC_POWER_ESTIMATE_PID_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_PowerEstimatePID_pack(app_canTx_VC_PowerEstimatePID_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_PIDSlipRatio_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_PID_SLIP_RATIO_ID;
    tx_msg.dlc = CANMSG_VC_PID_SLIP_RATIO_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_PIDSlipRatio_pack(app_canTx_VC_PIDSlipRatio_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_SlipRatios_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_SLIP_RATIOS_ID;
    tx_msg.dlc = CANMSG_VC_SLIP_RATIOS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_SlipRatios_pack(app_canTx_VC_SlipRatios_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_ActiveDiffOutputs_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ACTIVE_DIFF_OUTPUTS_ID;
    tx_msg.dlc = CANMSG_VC_ACTIVE_DIFF_OUTPUTS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_ActiveDiffOutputs_pack(app_canTx_VC_ActiveDiffOutputs_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_PowerLimit_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_POWER_LIMIT_ID;
    tx_msg.dlc = CANMSG_VC_POWER_LIMIT_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_PowerLimit_pack(app_canTx_VC_PowerLimit_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_VehicleDynamics_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_VEHICLE_DYNAMICS_ID;
    tx_msg.dlc = CANMSG_VC_VEHICLE_DYNAMICS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_VehicleDynamics_pack(app_canTx_VC_VehicleDynamics_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_EllipseEkfNavVelocity_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_ID;
    tx_msg.dlc = CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_EllipseEkfNavVelocity_pack(app_canTx_VC_EllipseEkfNavVelocity_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_Voltages_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_VOLTAGES_ID;
    tx_msg.dlc = CANMSG_VC_VOLTAGES_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_Voltages_pack(app_canTx_VC_Voltages_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_CurrentSensing_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_CURRENT_SENSING_ID;
    tx_msg.dlc = CANMSG_VC_CURRENT_SENSING_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_CurrentSensing_pack(app_canTx_VC_CurrentSensing_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_Efuse_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_EFUSE_ID;
    tx_msg.dlc = CANMSG_VC_EFUSE_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_Efuse_pack(app_canTx_VC_Efuse_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_ImuLinearAcceleration_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_IMU_LINEAR_ACCELERATION_ID;
    tx_msg.dlc = CANMSG_VC_IMU_LINEAR_ACCELERATION_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_ImuLinearAcceleration_pack(app_canTx_VC_ImuLinearAcceleration_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_CoolantPumpFlowRate_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_COOLANT_PUMP_FLOW_RATE_ID;
    tx_msg.dlc = CANMSG_VC_COOLANT_PUMP_FLOW_RATE_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_CoolantPumpFlowRate_pack(app_canTx_VC_CoolantPumpFlowRate_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_VCShdnNodeStatus_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_VC_SHDN_NODE_STATUS_ID;
    tx_msg.dlc = CANMSG_VC_VC_SHDN_NODE_STATUS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_VCShdnNodeStatus_pack(app_canTx_VC_VCShdnNodeStatus_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_GlobalShdnNodeStatus_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_ID;
    tx_msg.dlc = CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_GlobalShdnNodeStatus_pack(app_canTx_VC_GlobalShdnNodeStatus_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_PedalPercentage_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_PEDAL_PERCENTAGE_ID;
    tx_msg.dlc = CANMSG_VC_PEDAL_PERCENTAGE_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_PedalPercentage_pack(app_canTx_VC_PedalPercentage_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_Warnings_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_WARNINGS_ID;
    tx_msg.dlc = CANMSG_VC_WARNINGS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_Warnings_pack(app_canTx_VC_Warnings_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_Faults_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_FAULTS_ID;
    tx_msg.dlc = CANMSG_VC_FAULTS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_Faults_pack(app_canTx_VC_Faults_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_WarningsCounts_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_WARNINGS_COUNTS_ID;
    tx_msg.dlc = CANMSG_VC_WARNINGS_COUNTS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_WarningsCounts_pack(app_canTx_VC_WarningsCounts_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}
static void io_canTx_VC_FaultsCounts_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_VC_FAULTS_COUNTS_ID;
    tx_msg.dlc = CANMSG_VC_FAULTS_COUNTS_BYTES;
    #ifndef THREAD_SAFE_CAN_PACKING
    lock();
    #endif
    app_canUtils_VC_FaultsCounts_pack(app_canTx_VC_FaultsCounts_getData(), tx_msg.data);
    #ifndef THREAD_SAFE_CAN_PACKING
    unlock();
    #endif
    
    if (can_mode_bus1 & (CanMode_bus1_default))
    {
        transmit_func_bus1(&tx_msg);
    }
    
    if (can_mode_bus2 & (CanMode_bus2_default))
    {
        transmit_func_bus2(&tx_msg);
    }
    
}



/* --------------------- Public Function Definitions ---------------------- */

void io_canTx_init( 
    void (*transmit_bus1_msg_func)(const JsonCanMsg*),
    void (*transmit_bus2_msg_func)(const JsonCanMsg*)
    
)
{
    transmit_func_bus1 = transmit_bus1_msg_func;
    transmit_func_bus2 = transmit_bus2_msg_func;
    
}


void io_canTx_enableMode_bus1(CanMode_bus1 mode, bool enable)
{
    if (enable)
    {
        can_mode_bus1 |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_bus1 &= ~((uint32_t)mode); // Disable mode
    }
}

void io_canTx_enableMode_bus2(CanMode_bus2 mode, bool enable)
{
    if (enable)
    {
        can_mode_bus2 |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_bus2 &= ~((uint32_t)mode); // Disable mode
    }
}




void io_canTx_enqueue1HzMsgs()
{
    io_canTx_VC_EllipseEulerAngles_sendPeriodic();
    io_canTx_VC_PowerEstimate_sendPeriodic();
    io_canTx_VC_PowerEstimatePID_sendPeriodic();
    io_canTx_VC_PIDSlipRatio_sendPeriodic();
    io_canTx_VC_SlipRatios_sendPeriodic();
    io_canTx_VC_ActiveDiffOutputs_sendPeriodic();
    io_canTx_VC_CoolantPumpFlowRate_sendPeriodic();
    io_canTx_VC_Warnings_sendPeriodic();
    io_canTx_VC_WarningsCounts_sendPeriodic();
}

void io_canTx_enqueue100HzMsgs()
{
    io_canTx_VC_LeftInverterCommand_sendPeriodic();
    io_canTx_VC_RightInverterCommand_sendPeriodic();
    io_canTx_VC_AlertsContext_sendPeriodic();
    io_canTx_VC_PowerLimit_sendPeriodic();
}

void io_canTx_enqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CANMSG_VC_VITALS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    
    
    
    
    if (time_ms % CANMSG_VC_ELLIPSE_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_ELLIPSE_TIME_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_VC_COMMIT_INFO_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    
    
    
    
    
    if (time_ms % CANMSG_VC_VEHICLE_DYNAMICS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_VOLTAGES_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_CURRENT_SENSING_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_EFUSE_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_IMU_LINEAR_ACCELERATION_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_VC_VC_SHDN_NODE_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_VC_PEDAL_PERCENTAGE_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_VC_FAULTS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_VC_FAULTS_COUNTS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
}
