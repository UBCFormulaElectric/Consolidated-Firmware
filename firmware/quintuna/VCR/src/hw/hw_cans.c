#include "hw_cans.h"
#include "io_canMsg.h"
#include "io_canQueues.h"
#include "io_log.h"
#include "main.h"
#include <assert.h>
#include "app_jsoncan.h"
#include <io_canReroute.h>
#include <stdbool.h>

// NOTE: can2 refers to the CAN2_TX/RX on the altium
// I believe CAN2 on the altium refers to can3

void handleCallback(const CanMsg *rx_msg)
{
    // io_bootloaderReroute_reRoute(rx_msg);

    JsonCanMsg json_can_msg = app_jsoncan_copyFromCanMsg(rx_msg);

    io_canReroute_can1(&json_can_msg);
    io_canReroute_can2(&json_can_msg);
    io_canReroute_can3(&json_can_msg);

    // Doesnt' receive anything via JSONCAN.
}

CanHandle fd_can  = { .hcan = &hfdcan2, .bus_num = 1, .receive_callback = handleCallback };
CanHandle sx_can  = { .hcan = &hfdcan1, .bus_num = 2, .receive_callback = handleCallback };
CanHandle inv_can = { .hcan = &hfdcan3, .bus_num = 3, .receive_callback = handleCallback };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == fd_can.hcan)
    {
        return &fd_can;
    }
    else if (hfdcan == sx_can.hcan)
    {
        return &sx_can;
    }
    else if (hfdcan == inv_can.hcan)
    {
        return &inv_can;
    }
    else
    {
        assert(false);
        return NULL;
    }
}
