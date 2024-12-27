#pragma once

#include "io_canRx.h"
#include "io_canTx.h"

// Interface between sil and canbus.
// Hook for can to transmit a message via fakeCan.
void sil_txCallback(const JsonCanMsg *msg);

// Main SIL loop logic.
void sil_main(
    int   argc,
    char *argv[],
    void (*tasks_init)(),
    void (*tasks_1Hz)(uint32_t timeMs),
    void (*tasks_100Hz)(uint32_t timeMs),
    void (*tasks_1kHz)(uint32_t timeMs));