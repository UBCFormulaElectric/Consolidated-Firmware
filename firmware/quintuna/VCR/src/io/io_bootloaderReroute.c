#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
#include <stdint.h>
#include <string.h>

static void (*transmit_func_can2)(const CanMsg *tx_msg);

#define FSM_BOOTCONFIG 0x10000001
#define RSM_BOOTCONFIG 0x14000000
#define CRIT_BOOTCONFIG 0x18000000

void io_bootloadeReroute_init(
    void (*transmit_can2_msg_func)(const CanMsg *)){
    transmit_func_can2 = transmit_can2_msg_func;
}

void io_bootloaderReroute_reRoute(const CanMsg *msg)
{
    switch ((msg->std_id & 0x1FFFFFFC))
    {
        case (FSM_BOOTCONFIG):
            transmit_func_can2(msg);
            break;
        case (RSM_BOOTCONFIG):
            transmit_func_can2(msg);
            break;
        case (CRIT_BOOTCONFIG):
            transmit_func_can2(msg);
            break;
        default:
            break;
    }
}
