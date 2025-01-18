#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    const uint16_t address;
} power_rail;

extern const power_rail bat;
extern const power_rail boost;
extern const power_rail acc;
extern const power_rail ext;

bool io_power_monitor_init();

float io_power_monitor_read_voltage(const power_rail *voltage_address);
