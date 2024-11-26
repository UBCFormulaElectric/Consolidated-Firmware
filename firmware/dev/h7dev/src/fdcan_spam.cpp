#include "fdcan_demo.h"
#include <cstdint>

extern "C" {
#include "io_can.h"
#include "cmsis_os.h"
}

void fd_can_demo_tick() {
    static uint64_t i = 0;
    uint8_t i8[8];
    *reinterpret_cast<uint64_t *>(&i8) = i;
    const CanMsg msg = { .std_id = 0, .dlc = 8, .data = { i8[7], i8[6], i8[5], i8[4], i8[3], i8[2], i8[1], i8[0] } };
    io_can_pushTxMsgToQueue(&msg);
    i++;
    osDelay(1);
}
