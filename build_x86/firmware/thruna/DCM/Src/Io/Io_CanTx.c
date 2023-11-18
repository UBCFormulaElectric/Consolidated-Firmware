/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "Io_CanTx.h"
#include <string.h>
#include <FreeRTOS.h>
#include <portmacro.h>
#include "Io_SharedCanMsg.h"
#include "Io_SharedCan.h"
#include "App_CanTx.h"
#include "App_CanUtils.h"
/* --------------------------- Static Variables --------------------------- */

static uint32_t can_mode;

/* --------------------- Static Function Definitions ---------------------- */

static void Io_CanTx_DCM_Vitals_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_VITALS_ID;
        tx_msg.dlc = CAN_MSG_DCM_VITALS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_Vitals_Pack(App_CanTx_DCM_Vitals_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_LeftInverterCommand_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_LEFT_INVERTER_COMMAND_ID;
        tx_msg.dlc = CAN_MSG_DCM_LEFT_INVERTER_COMMAND_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_LeftInverterCommand_Pack(App_CanTx_DCM_LeftInverterCommand_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_RightInverterCommand_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_RIGHT_INVERTER_COMMAND_ID;
        tx_msg.dlc = CAN_MSG_DCM_RIGHT_INVERTER_COMMAND_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_RightInverterCommand_Pack(App_CanTx_DCM_RightInverterCommand_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_AlertsContext_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ALERTS_CONTEXT_ID;
        tx_msg.dlc = CAN_MSG_DCM_ALERTS_CONTEXT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_AlertsContext_Pack(App_CanTx_DCM_AlertsContext_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_EllipseStatus_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ELLIPSE_STATUS_ID;
        tx_msg.dlc = CAN_MSG_DCM_ELLIPSE_STATUS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_EllipseStatus_Pack(App_CanTx_DCM_EllipseStatus_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_EllipseTime_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ELLIPSE_TIME_ID;
        tx_msg.dlc = CAN_MSG_DCM_ELLIPSE_TIME_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_EllipseTime_Pack(App_CanTx_DCM_EllipseTime_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_EllipseAcceleration_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ELLIPSE_ACCELERATION_ID;
        tx_msg.dlc = CAN_MSG_DCM_ELLIPSE_ACCELERATION_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_EllipseAcceleration_Pack(App_CanTx_DCM_EllipseAcceleration_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_EllipseAngularVelocity_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ELLIPSE_ANGULAR_VELOCITY_ID;
        tx_msg.dlc = CAN_MSG_DCM_ELLIPSE_ANGULAR_VELOCITY_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_EllipseAngularVelocity_Pack(App_CanTx_DCM_EllipseAngularVelocity_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_EllipseEulerAngles_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_ELLIPSE_EULER_ANGLES_ID;
        tx_msg.dlc = CAN_MSG_DCM_ELLIPSE_EULER_ANGLES_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_EllipseEulerAngles_Pack(App_CanTx_DCM_EllipseEulerAngles_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_CommitInfo_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_COMMIT_INFO_ID;
        tx_msg.dlc = CAN_MSG_DCM_COMMIT_INFO_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_CommitInfo_Pack(App_CanTx_DCM_CommitInfo_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_Warnings_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_WARNINGS_ID;
        tx_msg.dlc = CAN_MSG_DCM_WARNINGS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_Warnings_Pack(App_CanTx_DCM_Warnings_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_Faults_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_FAULTS_ID;
        tx_msg.dlc = CAN_MSG_DCM_FAULTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_Faults_Pack(App_CanTx_DCM_Faults_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_WarningsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_WARNINGS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_DCM_WARNINGS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_WarningsCounts_Pack(App_CanTx_DCM_WarningsCounts_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DCM_FaultsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DCM_FAULTS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_DCM_FAULTS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DCM_FaultsCounts_Pack(App_CanTx_DCM_FaultsCounts_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

/* --------------------- Public Function Definitions ---------------------- */

void Io_CanTx_EnableMode(CanMode mode, bool enable)
{
    if (enable)
    {
        can_mode |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode &= ~((uint32_t)mode); // Disable mode
    }
}

void Io_CanTx_Enqueue1HzMsgs()
{
    Io_CanTx_DCM_AlertsContext_SendPeriodic();
    Io_CanTx_DCM_Warnings_SendPeriodic();
    Io_CanTx_DCM_WarningsCounts_SendPeriodic();
}

void Io_CanTx_Enqueue100HzMsgs()
{
    Io_CanTx_DCM_LeftInverterCommand_SendPeriodic();
    Io_CanTx_DCM_RightInverterCommand_SendPeriodic();
    Io_CanTx_DCM_EllipseAcceleration_SendPeriodic();
    Io_CanTx_DCM_EllipseAngularVelocity_SendPeriodic();
    Io_CanTx_DCM_EllipseEulerAngles_SendPeriodic();
}

void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_DCM_VITALS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_Vitals_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DCM_ELLIPSE_STATUS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_EllipseStatus_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DCM_ELLIPSE_TIME_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_EllipseTime_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DCM_COMMIT_INFO_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_CommitInfo_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DCM_FAULTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_Faults_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DCM_FAULTS_COUNTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DCM_FaultsCounts_SendPeriodic();
    }
    
}

