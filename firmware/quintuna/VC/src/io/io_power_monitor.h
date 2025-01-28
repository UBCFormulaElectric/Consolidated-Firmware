#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef TARGET_EMBEDDED
typedef struct
{
    const uint8_t address;
} PowerRail;

#else
#include "app_utils.h"
EMPTY_STRUCT(PowerRail)
#endif

extern const PowerRail bat;
extern const PowerRail boost;
extern const PowerRail acc;
extern const PowerRail ext;

bool io_power_monitor_init();

float io_power_monitor_read_voltage(const PowerRail *voltage_address);
