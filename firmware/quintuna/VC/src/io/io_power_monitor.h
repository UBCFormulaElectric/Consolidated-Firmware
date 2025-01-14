#pragma once

typedef struct
{
    uint8_t VBAT;
    uint8_t V_BOOST;
    uint8_t V_ACC;
    uint8_t V_EXT;
} voltage_bus;

bool io_power_monitor_init();

uint8_t io_power_monitor_read(voltage_bus voltage_address);
