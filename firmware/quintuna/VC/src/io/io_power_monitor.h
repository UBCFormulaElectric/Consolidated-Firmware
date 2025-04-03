#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef TARGET_EMBEDDED
typedef struct
{
    const uint8_t bus_address;
    const uint8_t sense_address;
    const uint8_t power_address;
} PowerRail;

#else
#include "app_utils.h"
EMPTY_STRUCT(PowerRail)
#endif

extern const PowerRail ext;
extern const PowerRail acc;

// TODO: add comments and decriptions for how deez work
bool io_power_monitor_init();

void io_power_monitor_refresh();

float io_power_monitor_read_voltage(const PowerRail *voltage_address);

float io_power_monitor_read_current(const PowerRail *current_address);

float io_power_monitor_read_power(const PowerRail *power_address);

uint32_t io_power_monitor_alert_status();
