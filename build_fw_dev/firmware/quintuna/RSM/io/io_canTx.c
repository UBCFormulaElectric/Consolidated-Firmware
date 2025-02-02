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
#include <FreeRTOS.h>
#include <portmacro.h>
#endif

/* --------------------------- Static Variables --------------------------- */

static uint32_t can_mode;
static void (*transmit_func)(const JsonCanMsg* tx_msg);

/* --------------------- Static Function Definitions ---------------------- */

static void io_canTx_RSM_Vitals_sendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        JsonCanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(JsonCanMsg));
        tx_msg.std_id = CAN_MSG_RSM_VITALS_ID;
        tx_msg.dlc = CAN_MSG_RSM_VITALS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        app_canUtils_RSM_Vitals_pack(app_canTx_RSM_Vitals_getData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void io_canTx_RSM_Warnings_sendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        JsonCanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(JsonCanMsg));
        tx_msg.std_id = CAN_MSG_RSM_WARNINGS_ID;
        tx_msg.dlc = CAN_MSG_RSM_WARNINGS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        app_canUtils_RSM_Warnings_pack(app_canTx_RSM_Warnings_getData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void io_canTx_RSM_Faults_sendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        JsonCanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(JsonCanMsg));
        tx_msg.std_id = CAN_MSG_RSM_FAULTS_ID;
        tx_msg.dlc = CAN_MSG_RSM_FAULTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        app_canUtils_RSM_Faults_pack(app_canTx_RSM_Faults_getData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void io_canTx_RSM_WarningsCounts_sendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        JsonCanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(JsonCanMsg));
        tx_msg.std_id = CAN_MSG_RSM_WARNINGS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_RSM_WARNINGS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        app_canUtils_RSM_WarningsCounts_pack(app_canTx_RSM_WarningsCounts_getData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

static void io_canTx_RSM_FaultsCounts_sendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        JsonCanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(JsonCanMsg));
        tx_msg.std_id = CAN_MSG_RSM_FAULTS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_RSM_FAULTS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortEnterCritical();
        #endif
        app_canUtils_RSM_FaultsCounts_pack(app_canTx_RSM_FaultsCounts_getData(), tx_msg.data);
        #ifndef THREAD_SAFE_CAN_PACKING
        vPortExitCritical();
        #endif
        
        // Append msg to TX FIFO
        transmit_func(&tx_msg);
    }
}

/* --------------------- Public Function Definitions ---------------------- */

void io_canTx_init(void (*transmit_tx_msg_func)(const JsonCanMsg*))
{
    transmit_func = transmit_tx_msg_func;
}

void io_canTx_enableMode(CanMode mode, bool enable)
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

void io_canTx_enqueue1HzMsgs()
{
    io_canTx_RSM_Warnings_sendPeriodic();
    io_canTx_RSM_WarningsCounts_sendPeriodic();
}

void io_canTx_enqueue100HzMsgs()
{
}

void io_canTx_enqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_RSM_VITALS_CYCLE_TIME_MS == 0)
    {
        io_canTx_RSM_Vitals_sendPeriodic();
    }
    
    if (time_ms % CAN_MSG_RSM_FAULTS_CYCLE_TIME_MS == 0)
    {
        io_canTx_RSM_Faults_sendPeriodic();
    }
    
    if (time_ms % CAN_MSG_RSM_FAULTS_COUNTS_CYCLE_TIME_MS == 0)
    {
        io_canTx_RSM_FaultsCounts_sendPeriodic();
    }
    
}

