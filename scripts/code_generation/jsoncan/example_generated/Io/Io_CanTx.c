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

static void Io_CanTx_JctVitals_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT | CAN_MODE_DEBUG))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_VITALS_ID;
    tx_msg.dlc = CAN_MSG_JCT_VITALS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JctVitals_Pack(App_CanTx_JctVitals_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

static void Io_CanTx_JctAirShutdownErrors_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_ID;
    tx_msg.dlc = CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JctAirShutdownErrors_Pack(App_CanTx_JctAirShutdownErrors_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

static void Io_CanTx_JctMotorShutdownErrors_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_ID;
    tx_msg.dlc = CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JctMotorShutdownErrors_Pack(App_CanTx_JctMotorShutdownErrors_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

static void Io_CanTx_JctStatus_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_STATUS_ID;
    tx_msg.dlc = CAN_MSG_JCT_STATUS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JctStatus_Pack(App_CanTx_JctStatus_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

static void Io_CanTx_JCT_Warnings_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_WARNINGS_ID;
    tx_msg.dlc = CAN_MSG_JCT_WARNINGS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JCT_Warnings_Pack(App_CanTx_JCT_Warnings_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

static void Io_CanTx_JCT_Faults_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_FAULTS_ID;
    tx_msg.dlc = CAN_MSG_JCT_FAULTS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JCT_Faults_Pack(App_CanTx_JCT_Faults_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
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
    Io_CanTx_JctVitals_SendPeriodic();
    Io_CanTx_JctAirShutdownErrors_SendPeriodic();
    Io_CanTx_JCT_Warnings_SendPeriodic();
}

void Io_CanTx_Enqueue100HzMsgs()
{
    Io_CanTx_JCT_Faults_SendPeriodic();
}

void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_JctMotorShutdownErrors_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_JCT_STATUS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_JctStatus_SendPeriodic();
    }
    
}

void Io_CanTx_JctWarnings_SendAperiodic()
{
    if (can_mode & (CAN_MODE_DEBUG))
    {
        return;
    }
    
    // Prepare msg header
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(struct CanMsg));
    tx_msg.std_id = CAN_MSG_JCT_WARNINGS_ID;
    tx_msg.dlc = CAN_MSG_JCT_WARNINGS_BYTES;
    
    // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
    vPortEnterCritical();
    App_CanUtils_JctWarnings_Pack(App_CanTx_JctWarnings_GetData(), tx_msg.data);
    vPortExitCritical();
    
    // Append msg to TX FIFO
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

