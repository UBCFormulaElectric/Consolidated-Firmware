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

static void Io_CanTx_BMS_Vitals_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_VITALS_ID;
        tx_msg.dlc = CAN_MSG_BMS_VITALS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_Vitals_Pack(App_CanTx_BMS_Vitals_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_AlertsContext_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_ALERTS_CONTEXT_ID;
        tx_msg.dlc = CAN_MSG_BMS_ALERTS_CONTEXT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_AlertsContext_Pack(App_CanTx_BMS_AlertsContext_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_ImdStatus_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_IMD_STATUS_ID;
        tx_msg.dlc = CAN_MSG_BMS_IMD_STATUS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_ImdStatus_Pack(App_CanTx_BMS_ImdStatus_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_ImdData_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_IMD_DATA_ID;
        tx_msg.dlc = CAN_MSG_BMS_IMD_DATA_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_ImdData_Pack(App_CanTx_BMS_ImdData_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_ImdPwmOutput_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_IMD_PWM_OUTPUT_ID;
        tx_msg.dlc = CAN_MSG_BMS_IMD_PWM_OUTPUT_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_ImdPwmOutput_Pack(App_CanTx_BMS_ImdPwmOutput_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_Charger_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_CHARGER_ID;
        tx_msg.dlc = CAN_MSG_BMS_CHARGER_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_Charger_Pack(App_CanTx_BMS_Charger_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_OkStatuses_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_OK_STATUSES_ID;
        tx_msg.dlc = CAN_MSG_BMS_OK_STATUSES_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_OkStatuses_Pack(App_CanTx_BMS_OkStatuses_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_Contactors_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_CONTACTORS_ID;
        tx_msg.dlc = CAN_MSG_BMS_CONTACTORS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_Contactors_Pack(App_CanTx_BMS_Contactors_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_CellTemperatures_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_CELL_TEMPERATURES_ID;
        tx_msg.dlc = CAN_MSG_BMS_CELL_TEMPERATURES_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_CellTemperatures_Pack(App_CanTx_BMS_CellTemperatures_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_CellVoltages_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_CELL_VOLTAGES_ID;
        tx_msg.dlc = CAN_MSG_BMS_CELL_VOLTAGES_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_CellVoltages_Pack(App_CanTx_BMS_CellVoltages_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_CellStats_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_CELL_STATS_ID;
        tx_msg.dlc = CAN_MSG_BMS_CELL_STATS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_CellStats_Pack(App_CanTx_BMS_CellStats_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_TractiveSystem_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_TRACTIVE_SYSTEM_ID;
        tx_msg.dlc = CAN_MSG_BMS_TRACTIVE_SYSTEM_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_TractiveSystem_Pack(App_CanTx_BMS_TractiveSystem_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_PackVoltage_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_PACK_VOLTAGE_ID;
        tx_msg.dlc = CAN_MSG_BMS_PACK_VOLTAGE_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_PackVoltage_Pack(App_CanTx_BMS_PackVoltage_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_AvailablePower_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_AVAILABLE_POWER_ID;
        tx_msg.dlc = CAN_MSG_BMS_AVAILABLE_POWER_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_AvailablePower_Pack(App_CanTx_BMS_AvailablePower_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_AuxThermistors_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_AUX_THERMISTORS_ID;
        tx_msg.dlc = CAN_MSG_BMS_AUX_THERMISTORS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_AuxThermistors_Pack(App_CanTx_BMS_AuxThermistors_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_CommitInfo_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_COMMIT_INFO_ID;
        tx_msg.dlc = CAN_MSG_BMS_COMMIT_INFO_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_CommitInfo_Pack(App_CanTx_BMS_CommitInfo_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_Warnings_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_WARNINGS_ID;
        tx_msg.dlc = CAN_MSG_BMS_WARNINGS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_Warnings_Pack(App_CanTx_BMS_Warnings_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_Faults_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_FAULTS_ID;
        tx_msg.dlc = CAN_MSG_BMS_FAULTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_Faults_Pack(App_CanTx_BMS_Faults_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_WarningsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_WARNINGS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_BMS_WARNINGS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_WarningsCounts_Pack(App_CanTx_BMS_WarningsCounts_GetData(), tx_msg.data);
        vPortExitCritical();
        
        // Append msg to TX FIFO
        Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
    }
}

static void Io_CanTx_BMS_FaultsCounts_SendPeriodic()
{
    if (can_mode & (CAN_MODE_DEFAULT))
    {
        // Prepare msg header
        struct CanMsg tx_msg;
        memset(&tx_msg, 0, sizeof(struct CanMsg));
        tx_msg.std_id = CAN_MSG_BMS_FAULTS_COUNTS_ID;
        tx_msg.dlc = CAN_MSG_BMS_FAULTS_COUNTS_BYTES;
        
        // Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)
        vPortEnterCritical();
        App_CanUtils_BMS_FaultsCounts_Pack(App_CanTx_BMS_FaultsCounts_GetData(), tx_msg.data);
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
    Io_CanTx_BMS_AlertsContext_SendPeriodic();
    Io_CanTx_BMS_ImdStatus_SendPeriodic();
    Io_CanTx_BMS_ImdData_SendPeriodic();
    Io_CanTx_BMS_ImdPwmOutput_SendPeriodic();
    Io_CanTx_BMS_Charger_SendPeriodic();
    Io_CanTx_BMS_Warnings_SendPeriodic();
    Io_CanTx_BMS_WarningsCounts_SendPeriodic();
}

void Io_CanTx_Enqueue100HzMsgs()
{
    Io_CanTx_BMS_OkStatuses_SendPeriodic();
    Io_CanTx_BMS_Contactors_SendPeriodic();
}

void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CAN_MSG_BMS_VITALS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_Vitals_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_CELL_TEMPERATURES_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_CellTemperatures_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_CELL_VOLTAGES_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_CellVoltages_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_CELL_STATS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_CellStats_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_TRACTIVE_SYSTEM_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_TractiveSystem_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_PACK_VOLTAGE_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_PackVoltage_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_AVAILABLE_POWER_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_AvailablePower_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_AUX_THERMISTORS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_AuxThermistors_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_COMMIT_INFO_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_CommitInfo_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_FAULTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_Faults_SendPeriodic();
    }
    
    if (time_ms % CAN_MSG_BMS_FAULTS_COUNTS_CYCLE_TIME_MS == 0)
    {
        Io_CanTx_BMS_FaultsCounts_SendPeriodic();
    }
    
}

