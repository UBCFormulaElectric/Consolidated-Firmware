#pragma once

#include "io_canRx.h"
#include "io_canTx.h"

void sil_txCallback(const JsonCanMsg *msg);

void sil_main(
    int argc, char *argv[],
    void tasks_init(),
    void tasks_1Hz(uint32_t time_ms),
    void tasks_100Hz(uint32_t time_ms),
    void tasks_1kHz(uint32_t time_ms));