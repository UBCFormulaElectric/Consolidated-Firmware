#pragma once

#include "io_switch.h"

typedef struct
{
    const Switch *const start_switch;
    const Switch *const regen_switch;
    const Switch *const torquevec_switch;
} Switches;

void app_switches_init(const Switches *switches_in);

void app_switches_update(void);
