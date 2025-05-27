#include "fdcan_spam.h"
#include <stdint.h>
#include "io_canQueue.h"
#include "cmsis_os.h"

void fd_can_demo_tick()
{
    static uint64_t i   = 0;
    const CanMsg    msg = {
           .std_id    = 0,
           .dlc       = 8,
           .timestamp = 0,
           .data      = { .data64 = { i } },
           .bus       = 0,
    };
    io_canQueue_pushTx(&msg);
    i++;
    osDelay(1);
}
