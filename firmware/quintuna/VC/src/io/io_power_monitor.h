#pragma once

typedef struct
{
    const uint8_t VBAT;
    const uint8_t V_BOOST;
    const uint8_t V_ACC;
    const uint8_t V_EXT;
} voltage_bus;

bool io_power_monitor_init();

uint8_t io_power_monitor_read(voltage_bus voltage_address);
