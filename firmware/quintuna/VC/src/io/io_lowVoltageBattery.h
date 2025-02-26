#pragma once

#include "hw_gpio.h"
#include "cmsis_os2.h"

extern osSemaphoreId_t bat_mtr_sem;

extern const uint16_t ACCUMULATED_CHARGE_COMMAND;
extern const uint16_t CELL0_VOLTAGE_COMMAND;
extern const uint16_t CELL1_VOLTAGE_COMMAND;
extern const uint16_t CELL2_VOLTAGE_COMMAND;
extern const uint16_t CELL4_VOLTAGE_COMMAND;
extern const uint16_t STACK_VOLTAGE_COMMAND;

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
