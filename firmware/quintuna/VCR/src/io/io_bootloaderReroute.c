#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
#include "app_utils.h"
#include "io_canQueues.h"
#include <stdint.h>
#include <string.h>

#define STATUS_10HZ_ID_LOWBITS (0x0)
#define START_UPDATE_ID_LOWBITS (0x1)
#define UPDATE_ACK_ID_LOWBITS (0x2)
#define GO_TO_APP_LOWBITS (0x3)
#define ERASE_SECTOR_ID_LOWBITS (0x4)
#define ERASE_SECTOR_COMPLETE_ID_LOWBITS (0x5)
#define PROGRAM_ID_LOWBITS (0x6)
#define VERIFY_ID_LOWBITS (0x7)
#define APP_VALIDITY_ID_LOWBITS (0x8)
#define GO_TO_BOOT (0x9)

static void (*transmit_func_can2)(const CanMsg *tx_msg);
static void (*transmit_func_can1)(const CanMsg *tx_msg);

#define FSM_BOOTCONFIG 0x10000000
#define RSM_BOOTCONFIG 0x14000000
#define CRIT_BOOTCONFIG 0x18000000

static void bootloader_reroute_can2(const CanMsg *msg)
{
    CanMsg new_msg;

    memset(&new_msg, 0, sizeof(CanMsg));

    new_msg.std_id = msg->std_id;
    new_msg.dlc    = MAX(msg->dlc, 8);

    // We can do this as for bootloader the packets are never going to be larger than 8 bytes
    memcpy(&(new_msg.data), &(msg->data), sizeof(uint64_t));

    io_canQueue_pushTx(&sx_can_tx_queue, &new_msg);
}

static void bootloader_reroute_can1(const CanMsg *msg)
{
    CanMsg new_msg;

    memset(&new_msg, 0, sizeof(CanMsg));

    new_msg.std_id = msg->std_id;
    new_msg.dlc    = MAX(msg->dlc, 8);

    // We can do this as for bootloader the packets are never going to be larger than 8 bytes
    memcpy(&(new_msg.data), &(msg->data), sizeof(uint64_t));

    io_canQueue_pushTx(&fd_can_tx_queue, &new_msg);
}

static void routing_logic(const CanMsg *msg, const uint8_t message_id)
{
    if (message_id == START_UPDATE_ID_LOWBITS || message_id == ERASE_SECTOR_ID_LOWBITS ||
        message_id == PROGRAM_ID_LOWBITS || message_id == VERIFY_ID_LOWBITS || message_id == GO_TO_APP_LOWBITS ||
        message_id == GO_TO_BOOT)
    {
        bootloader_reroute_can2(msg);
    }
    else if (
        message_id == UPDATE_ACK_ID_LOWBITS || message_id == ERASE_SECTOR_COMPLETE_ID_LOWBITS ||
        message_id == APP_VALIDITY_ID_LOWBITS || message_id == STATUS_10HZ_ID_LOWBITS)
    {
        bootloader_reroute_can1(msg);
    }
}

void io_bootloaderReroute_reRoute(const CanMsg *msg)
{
    const uint64_t board_id   = msg->std_id & 0x1FFFFFC0;
    const uint8_t  message_id = msg->std_id & 0xF;
    switch (board_id)
    {
        case (FSM_BOOTCONFIG):
        case (RSM_BOOTCONFIG):
        case (CRIT_BOOTCONFIG):
            routing_logic(msg, message_id);
            break;
        default:
            break;
    }
}
