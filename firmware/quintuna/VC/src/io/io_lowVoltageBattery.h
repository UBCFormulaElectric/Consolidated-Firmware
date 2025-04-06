#pragma once

#include "hw_gpios.h"
#include <stdint.h>

typedef enum
{
    ACCUMULATED_CHARGE_COMMAND = 0x0076
} charge_cmd_t;

typedef enum
{
    BATTERY_STATUS = 0x12,
    CONTROL_STATUS = 0x00
} commands;
typedef enum
{
    CELL0_VOLTAGE_COMMAND = 0x14,
    CELL1_VOLTAGE_COMMAND = 0x16,
    CELL2_VOLTAGE_COMMAND = 0x1A,
    CELL4_VOLTAGE_COMMAND = 0x1C,
    STACK_VOLTAGE_COMMAND = 0x34
} voltage_cmd_t;

/**
 * @brief Initializes the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
bool io_lowVoltageBattery_init();

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
float io_lowVoltageBattery_get_SOC();

/**
 * @brief Gets the battery voltage.
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
uint16_t io_lowVoltageBattery_get_voltage(uint16_t voltage_cmd);

// intial setup for battery monitoring chip
bool io_lowVoltageBattery_initial_setup(void);

/**
 * @brief Handles releasing the semaphore after an interupt.
 *
 * @param GPIO_pin from the interupt.
 */
void io_lowVoltageBattery_completeAlert(uint16_t GPIO_pin);