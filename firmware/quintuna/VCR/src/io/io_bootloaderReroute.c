#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
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

void io_bootloadeReroute_init(
    void (*transmit_can2_msg_func)(const CanMsg *),
    void (*transmit_can1_msg_func)(const CanMsg *))
{
    transmit_func_can2 = transmit_can2_msg_func;
    transmit_func_can1 = transmit_can1_msg_func;
}

static void routing_logic(const CanMsg *msg, uint8_t message_id){
    if (message_id == START_UPDATE_ID_LOWBITS || message_id == ERASE_SECTOR_ID_LOWBITS ||
        message_id == PROGRAM_ID_LOWBITS || message_id == VERIFY_ID_LOWBITS ||
        message_id == GO_TO_APP_LOWBITS || message_id == GO_TO_BOOT)
    {
        transmit_func_can2(msg);
    }
    else if (
        message_id == UPDATE_ACK_ID_LOWBITS || message_id == ERASE_SECTOR_COMPLETE_ID_LOWBITS ||
        message_id == APP_VALIDITY_ID_LOWBITS || message_id == STATUS_10HZ_ID_LOWBITS)
    {
        transmit_func_can1(msg);
    }
}

void io_bootloaderReroute_reRoute(const CanMsg *msg)
{
    uint64_t board_id   = msg->std_id & 0x1FFFFFC0;
    uint8_t  message_id = msg->std_id & 0xF;
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
