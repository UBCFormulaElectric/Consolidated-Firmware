/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "Io_CanTx.h"
#include <string.h>
#include "App_CanTx.h"
#include "App_CanUtils.h"

#ifndef THREAD_SAFE_CAN_PACKING
#include <FreeRTOS.h>
#include <portmacro.h>
#endif

/* --------------------------- Static Variables --------------------------- */

static uint32_t can_mode;
static void (*transmit_func)(const CanMsg* tx_msg);

/* --------------------- Static Function Definitions ---------------------- */

static void Io_CanTx_FSM_Vitals_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_VITALS_ID;
        tx_msg.dlc = CAN_MSG_FSM_VITALS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Vitals_Pack(App_CanTx_FSM_Vitals_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_AlertsContext_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_ALERTS_CONTEXT_ID;
        tx_msg.dlc = CAN_MSG_FSM_ALERTS_CONTEXT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_AlertsContext_Pack(App_CanTx_FSM_AlertsContext_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Apps_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_APPS_ID;
        tx_msg.dlc = CAN_MSG_FSM_APPS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Apps_Pack(App_CanTx_FSM_Apps_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Brake_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_BRAKE_ID;
        tx_msg.dlc = CAN_MSG_FSM_BRAKE_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Brake_Pack(App_CanTx_FSM_Brake_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Coolant_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_COOLANT_ID;
        tx_msg.dlc = CAN_MSG_FSM_COOLANT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Coolant_Pack(App_CanTx_FSM_Coolant_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Steering_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_STEERING_ID;
        tx_msg.dlc = CAN_MSG_FSM_STEERING_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Steering_Pack(App_CanTx_FSM_Steering_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Wheels_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_WHEELS_ID;
        tx_msg.dlc = CAN_MSG_FSM_WHEELS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Wheels_Pack(App_CanTx_FSM_Wheels_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_CommitInfo_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_COMMIT_INFO_ID;
        tx_msg.dlc = CAN_MSG_FSM_COMMIT_INFO_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_CommitInfo_Pack(App_CanTx_FSM_CommitInfo_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Warnings_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_WARNINGS_ID;
        tx_msg.dlc = CAN_MSG_FSM_WARNINGS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Warnings_Pack(App_CanTx_FSM_Warnings_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_Faults_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_FAULTS_ID;
        tx_msg.dlc = CAN_MSG_FSM_FAULTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_Faults_Pack(App_CanTx_FSM_Faults_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_WarningsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_WARNINGS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_FSM_WARNINGS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_WarningsCounts_Pack(App_CanTx_FSM_WarningsCounts_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void Io_CanTx_FSM_FaultsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(CanMsg));
        tx_msg.std_id = CAN_MSG_FSM_FAULTS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_FSM_FAULTS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        App_CanUtils_FSM_FaultsCounts_Pack(App_CanTx_FSM_FaultsCounts_GetData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

/* --------------------- Public Function Definitions ---------------------- */

void Io_CanTx_Init(void (*transmit_tx_msg_func)(const CanMsg*))
{
    transmit_func = transmit_tx_msg_func;
}

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
    Io_CanTx_FSM_AlertsContext_SendPeriodic();
    Io_CanTx_FSM_Coolant_SendPeriodic();
    Io_CanTx_FSM_Warnings_SendPeriodic();
    Io_CanTx_FSM_WarningsCounts_SendPeriodic();
}

void Io_CanTx_Enqueue100HzMsgs()
{
}

void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_FSM_VITALS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Vitals_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_APPS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Apps_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_BRAKE_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Brake_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_STEERING_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Steering_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_WHEELS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Wheels_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_COMMIT_INFO_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_CommitInfo_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_FAULTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_Faults_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_FSM_FAULTS_COUNTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_FSM_FaultsCounts_SendPeriodic();
    }
    
}

