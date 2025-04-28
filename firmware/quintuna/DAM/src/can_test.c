#include "can_test.h"
#include "hw_cans.h"
#include <stdint.h>
#include "io_canQueue.h"
#include "cmsis_os.h"
#include "hw_fdcan.h"
#include "io_log.h"

void can_test_tick()
{
    static uint64_t i = 0;
    uint8_t         i8[8];
    *(uint64_t *)&i8 = i;

    CanMsg msg = {
        .std_id    = 0,
        .dlc       = 8,
        .timestamp = 0,
        .data      = { i8[7], i8[6], i8[5], i8[4], i8[3], i8[2], i8[1], i8[0] },
        .bus       = 0,
        .is_fd     = false,
    };

    CanMsg fd_msg = {
        .std_id    = 1,
        .dlc       = 15,
        .timestamp = 0,
        .data      = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9 },
        .bus       = 0,
        .is_fd     = true,
    };

    io_canQueue_init();

    while (1)
    {
        // hw_can_transmit(&can1, &msg);
        // hw_fdcan_transmit(&can1, &fd_msg);
        LOG_INFO("WAITING FOR MSG...");
        const CanMsg rx_msg = io_canQueue_popRx();
        LOG_INFO("CAN MSG OUTPUT: %d", rx_msg.data[0]);

        i++;
        osDelay(10);
    }
}