#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "configs/App_SharedSignalConfig.h"

#ifndef World
#error "Please define the 'World' type"
#endif

struct WaitSignal;

struct WaitSignalCallback
{
    // Wait duration before the callback function is called
    uint32_t wait_duration_ms;

    // The callback function to call when the wait has completed
    void (*function)(struct World *);
};

struct WaitSignal *App_SharedWaitSignal_Create(
    uint32_t initial_time_ms,
    bool (*is_high)(struct World *),
    struct World *            world,
    struct WaitSignalCallback callback);

void App_SharedWaitSignal_Destroy(struct WaitSignal * wait_signal);

void App_SharedWaitSignal_Update(
    struct WaitSignal *wait_signal,
    uint32_t           current_ms);
