#include "fdcan_spam.h"
#include <stdint.h>
#include "io_canQueue.h"
#include "cmsis_os.h"

void fd_can_demo_tick()
{
    static uint64_t i = 0;
    uint8_t         i8[8];
    *(uint64_t *)&i8 = i;

    const CanMsg msg = {
        .std_id    = 0,
        .dlc       = 8,
        .timestamp = 0,
        .data      = { i8[7], i8[6], i8[5], i8[4], i8[3], i8[2], i8[1], i8[0] },
        .bus       = 0,
    };
    io_canQueue_pushTx(&msg);
    i++;
    osDelay(1);
}
