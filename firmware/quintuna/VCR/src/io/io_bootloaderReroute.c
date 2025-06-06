#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
#include <stdint.h>
#include <string.h>

static void (*transmit_func_can1)(const CanMsg *tx_msg);
static void (*transmit_func_can2)(const CanMsg *tx_msg);
static void (*transmit_func_can3)(const CanMsg *tx_msg);

#define FSM_BOOTCONFIG 0x10000001
#define RSM_BOOTCONFIG 0x14000000
#define CRIT_BOOTCONFIG 0x18000000


void io_bootloadeReroute_init(
    void (*transmit_can1_msg_func)(const CanMsg *),
    void (*transmit_can2_msg_func)(const CanMsg *),
    void (*transmit_can3_msg_func)(const CanMsg *))
{
    transmit_func_can1 = transmit_can1_msg_func;
    transmit_func_can2 = transmit_can2_msg_func;
    transmit_func_can3 = transmit_can3_msg_func;
}

void io_bootloaderReroute_reRoute(const CanMsg *msg){
    CanMsg new_msg;
    memset(&new_msg, 0, sizeof(CanMsg));

    new_msg.std_id = msg->std_id;
    new_msg.dlc = 8;

    memcpy(&(new_msg.data), &(msg->data), sizeof(uint64_t));

    switch ((msg->std_id & 0x1FFFFFFC)) {
        case(FSM_BOOTCONFIG):
            transmit_func_can2(&new_msg);
            break;
        case(RSM_BOOTCONFIG):
            transmit_func_can2(&new_msg);
            break;
        case(CRIT_BOOTCONFIG):
            transmit_func_can2(&new_msg);
            break;
        default:
            break;
    }
}
