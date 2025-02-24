#pragma once

#include "io_switch.h"

typedef struct
{
    const Switch *const launch_control;
    const Switch *const torque_vectoring;
    const Switch *const regen;
    const Switch *const telem;
    const Switch *const push_drive;
} Switches;

void io_switches_init(const Switches *switches_in);

bool io_switches_launch_control_get(void);
bool io_switches_torque_vectoring_get(void);
bool io_switches_regen_get(void);
bool io_switches_telem_get(void);
bool io_switches_push_drive_get(void);
