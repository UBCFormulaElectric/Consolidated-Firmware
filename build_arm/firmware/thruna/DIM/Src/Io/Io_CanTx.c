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

static void Io_CanTx_DIM_Vitals_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DIM_VITALS_ID;
        tx_msg.dlc = CAN_MSG_DIM_VITALS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DIM_Vitals_Pack(App_CanTx_DIM_Vitals_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DIM_Switches_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DIM_SWITCHES_ID;
        tx_msg.dlc = CAN_MSG_DIM_SWITCHES_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DIM_Switches_Pack(App_CanTx_DIM_Switches_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DIM_AlertsContext_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DIM_ALERTS_CONTEXT_ID;
        tx_msg.dlc = CAN_MSG_DIM_ALERTS_CONTEXT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DIM_AlertsContext_Pack(App_CanTx_DIM_AlertsContext_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DIM_Warnings_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DIM_WARNINGS_ID;
        tx_msg.dlc = CAN_MSG_DIM_WARNINGS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DIM_Warnings_Pack(App_CanTx_DIM_Warnings_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_DIM_Faults_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_DIM_FAULTS_ID;
        tx_msg.dlc = CAN_MSG_DIM_FAULTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_DIM_Faults_Pack(App_CanTx_DIM_Faults_GetData(), tx_msg.data);
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
    Io_CanTx_DIM_AlertsContext_SendPeriodic();
    Io_CanTx_DIM_Warnings_SendPeriodic();
}

void Io_CanTx_Enqueue100HzMsgs()
{
}

void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_DIM_VITALS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DIM_Vitals_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DIM_SWITCHES_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DIM_Switches_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_DIM_FAULTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_DIM_Faults_SendPeriodic();
    }
    
}

