#pragma once

#include "io_switch.h"

typedef struct
{
    const Switch *const start_switch;
    const Switch *const regen_switch;
    const Switch *const torquevec_switch;
} Switches;

void io_switches_init(const Switches *switches_in);

bool io_switches_start_get(void);
bool io_switches_regen_get(void);
bool io_switches_torquevec_get(void);
