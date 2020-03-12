#include <string.h>
#include <FreeRTOS.h>
#include <portmacro.h>

#include "auto_generated/Io_CanTx.h"
#include "auto_generated/CanMsgs.h"
#include "auto_generated/App_CanTx.h"

#include "SharedCan.h"

void Io_CanTx_TransmitPeriodicMessages(
    struct CanTxInterface* can_tx_interface,
    const uint32_t current_ms)
{
    struct CanMsg tx_message;

    // Is it time to transmit this particular CAN message?
    if ((current_ms % CANMSGS_DCM_HEARTBEAT_CYCLE_TIME_MS) == 0)
    {
        memset(&tx_message, 0, sizeof(tx_message));

        // Prepare CAN message header
        tx_message.std_id = CANMSGS_DCM_HEARTBEAT_FRAME_ID;
        tx_message.dlc = CANMSGS_DCM_HEARTBEAT_LENGTH;

        // Prepare CAN message payload (The packing function isn't thread-safe
        // so we must guard it)
        portENTER_CRITICAL();
        CanMsgs_dcm_heartbeat_pack(
            &tx_message.data[0],
            App_CanTx_GetPeriodicMsgPointer_DCM_HEARTBEAT(can_tx_interface),
            tx_message.dlc);
        portEXIT_CRITICAL();

        Io_SharedCan_TxMessageQueueSendtoBack(&tx_message);
    }

    // Is it time to transmit this particular CAN message?
    if ((current_ms % CANMSGS_DCM_CAN_FIFO_OVERFLOW_CYCLE_TIME_MS) == 0)
    {
        memset(&tx_message, 0, sizeof(tx_message));

        // Prepare CAN message header
        tx_message.std_id = CANMSGS_DCM_CAN_FIFO_OVERFLOW_FRAME_ID;
        tx_message.dlc = CANMSGS_DCM_CAN_FIFO_OVERFLOW_LENGTH;

        // Prepare CAN message payload (The packing function isn't thread-safe
        // so we must guard it)
        portENTER_CRITICAL();
        CanMsgs_dcm_can_fifo_overflow_pack(
            &tx_message.data[0],
            App_CanTx_GetPeriodicMsgPointer_DCM_CAN_FIFO_OVERFLOW(can_tx_interface),
            tx_message.dlc);
        portEXIT_CRITICAL();

        Io_SharedCan_TxMessageQueueSendtoBack(&tx_message);
    }
}


void Io_CanTx_EnqueueNonPeriodicMsg_DCM_STARTUP(struct CanMsgs_dcm_startup_t* payload)
{
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(tx_msg));
    tx_msg.std_id = CANMSGS_DCM_STARTUP_FRAME_ID;
    tx_msg.dlc    = CANMSGS_DCM_STARTUP_LENGTH;
    memcpy(&tx_msg.data[0], &payload, CANMSGS_DCM_STARTUP_LENGTH);
    CanMsgs_dcm_startup_pack(&tx_msg.data[0], payload, CANMSGS_DCM_STARTUP_LENGTH);
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}

void Io_CanTx_EnqueueNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanMsgs_dcm_watchdog_timeout_t * payload)
{
    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(tx_msg));
    tx_msg.std_id = CANMSGS_DCM_STARTUP_FRAME_ID;
    tx_msg.dlc    = CANMSGS_DCM_STARTUP_LENGTH;
    memcpy(&tx_msg.data[0], &payload, CANMSGS_DCM_STARTUP_LENGTH);
    CanMsgs_dcm_watchdog_timeout_pack(&tx_msg.data[0], payload, CANMSGS_DCM_STARTUP_LENGTH);
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);
}
