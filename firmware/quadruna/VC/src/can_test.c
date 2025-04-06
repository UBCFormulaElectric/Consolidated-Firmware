#include "can_test.h"
#include "hw_cans.h"
#include <stdint.h>
#include "io_canQueue.h"
#include "cmsis_os.h"
#include "hw_fdcan.h"

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
        .dlc       = 8,
        .timestamp = 0,
        .data      = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff   },
        .bus       = 0,
        .is_fd     = true,
    };

    while(1) {
        hw_can_transmit(&can1, &msg);
        hw_fdcan_transmit(&can1, &fd_msg);
        i++;
        osDelay(10);
    }
}