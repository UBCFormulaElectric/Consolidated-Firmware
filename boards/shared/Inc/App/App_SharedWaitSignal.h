#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "configs/App_SharedWaitSignalConfig.h"

#ifndef World
#error "Please define the 'World' type"
#endif

struct WaitSignal;

struct WaitSignalCallback
{
    // Wait duration before the callback function is called
    uint32_t wait_duration_ms;

    // The callback function to call when the wait has completed
    void (*wait_complete_callback)(struct World *);
};

struct WaitSignal *App_SharedWaitSignal_Create(
    uint32_t initial_time_ms,
    bool (*start_wait)(struct World *),
    struct World *            world,
    struct WaitSignalCallback wait_signal_callback);

void App_SharedWaitSignal_Update(
    struct WaitSignal *wait_signal,
    uint32_t           current_ms);
