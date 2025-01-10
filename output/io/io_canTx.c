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

static uint32_t can_mode_;
static void (*transmit_func_)(const JsonCanMsg* tx_msg);

static uint32_t can_mode_;
static void (*transmit_func_)(const JsonCanMsg* tx_msg);


/* --------------------- Static Function Definitions ---------------------- */

static void io_canTx_BMS_Vitals_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_VITALS_ID;
    tx_msg.dlc = CANMSG_BMS_VITALS_BYTES;
    app_canUtils_BMS_Vitals_pack(app_canTx_BMS_Vitals_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_AlertsContext_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_ALERTS_CONTEXT_ID;
    tx_msg.dlc = CANMSG_BMS_ALERTS_CONTEXT_BYTES;
    app_canUtils_BMS_AlertsContext_pack(app_canTx_BMS_AlertsContext_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_ImdStatus_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_IMD_STATUS_ID;
    tx_msg.dlc = CANMSG_BMS_IMD_STATUS_BYTES;
    app_canUtils_BMS_ImdStatus_pack(app_canTx_BMS_ImdStatus_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_ImdData_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_IMD_DATA_ID;
    tx_msg.dlc = CANMSG_BMS_IMD_DATA_BYTES;
    app_canUtils_BMS_ImdData_pack(app_canTx_BMS_ImdData_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_Charger_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_CHARGER_ID;
    tx_msg.dlc = CANMSG_BMS_CHARGER_BYTES;
    app_canUtils_BMS_Charger_pack(app_canTx_BMS_Charger_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_BrusaControls_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_BRUSA_CONTROLS_ID;
    tx_msg.dlc = CANMSG_BMS_BRUSA_CONTROLS_BYTES;
    app_canUtils_BMS_BrusaControls_pack(app_canTx_BMS_BrusaControls_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_OkStatuses_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_OK_STATUSES_ID;
    tx_msg.dlc = CANMSG_BMS_OK_STATUSES_BYTES;
    app_canUtils_BMS_OkStatuses_pack(app_canTx_BMS_OkStatuses_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_Contactors_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_CONTACTORS_ID;
    tx_msg.dlc = CANMSG_BMS_CONTACTORS_BYTES;
    app_canUtils_BMS_Contactors_pack(app_canTx_BMS_Contactors_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_CellTemperatures_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_CELL_TEMPERATURES_ID;
    tx_msg.dlc = CANMSG_BMS_CELL_TEMPERATURES_BYTES;
    app_canUtils_BMS_CellTemperatures_pack(app_canTx_BMS_CellTemperatures_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_VoltageAndChargeStats_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID;
    tx_msg.dlc = CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_BYTES;
    app_canUtils_BMS_VoltageAndChargeStats_pack(app_canTx_BMS_VoltageAndChargeStats_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_CellStats_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_CELL_STATS_ID;
    tx_msg.dlc = CANMSG_BMS_CELL_STATS_BYTES;
    app_canUtils_BMS_CellStats_pack(app_canTx_BMS_CellStats_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_TractiveSystem_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_TRACTIVE_SYSTEM_ID;
    tx_msg.dlc = CANMSG_BMS_TRACTIVE_SYSTEM_BYTES;
    app_canUtils_BMS_TractiveSystem_pack(app_canTx_BMS_TractiveSystem_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_CommitInfo_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_COMMIT_INFO_ID;
    tx_msg.dlc = CANMSG_BMS_COMMIT_INFO_BYTES;
    app_canUtils_BMS_CommitInfo_pack(app_canTx_BMS_CommitInfo_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_OWC_Segment0to2_Status_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_ID;
    tx_msg.dlc = CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_BYTES;
    app_canUtils_BMS_OWC_Segment0to2_Status_pack(app_canTx_BMS_OWC_Segment0to2_Status_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_OWC_Segment3to4_Status_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_ID;
    tx_msg.dlc = CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_BYTES;
    app_canUtils_BMS_OWC_Segment3to4_Status_pack(app_canTx_BMS_OWC_Segment3to4_Status_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}
static void io_canTx_BMS_BMSShdnNodeStatus_sendPeriodic()
{
    JsonCanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(JsonCanMsg));
    tx_msg.std_id = CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID;
    tx_msg.dlc = CANMSG_BMS_BMS_SHDN_NODE_STATUS_BYTES;
    app_canUtils_BMS_BMSShdnNodeStatus_pack(app_canTx_BMS_BMSShdnNodeStatus_getData(), tx_msg.data);

    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
    if (can_mode_ & (CanMode__))
    {
        transmit_func_(&tx_msg);
    }
    
}



/* --------------------- Public Function Definitions ---------------------- */

void io_canTx_init( 
    void (*transmit__msg_func)(const JsonCanMsg*),
    void (*transmit__msg_func)(const JsonCanMsg*)
    
)
{
    transmit_func_ = transmit__msg_func;
    transmit_func_ = transmit__msg_func;
    
}


void io_canTx_enableMode_(CanMode_ mode, bool enable)
{
    if (enable)
    {
        can_mode_ |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_ &= ~((uint32_t)mode); // Disable mode
    }
}

void io_canTx_enableMode_(CanMode_ mode, bool enable)
{
    if (enable)
    {
        can_mode_ |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_ &= ~((uint32_t)mode); // Disable mode
    }
}




void io_canTx_enqueue1HzMsgs()
{
    io_canTx_BMS_AlertsContext_sendPeriodic();
    io_canTx_BMS_ImdData_sendPeriodic();
    io_canTx_BMS_CellTemperatures_sendPeriodic();
}

void io_canTx_enqueue100HzMsgs()
{
}

void io_canTx_enqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    if (time_ms % CANMSG_BMS_VITALS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_BMS_IMD_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_BMS_CHARGER_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_BRUSA_CONTROLS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_OK_STATUSES_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_CONTACTORS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
    if (time_ms % CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_CELL_STATS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_TRACTIVE_SYSTEM_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_COMMIT_INFO_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    if (time_ms % CANMSG_BMS_BMS_SHDN_NODE_STATUS_CYCLE_TIME == 0)
    {
        io_canTx_BMS_Vitals_sendPeriodic();
    }
    
}
