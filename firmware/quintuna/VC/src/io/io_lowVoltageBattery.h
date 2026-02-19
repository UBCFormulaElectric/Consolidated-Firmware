#pragma once

#include "app_utils.h"
#include "io_lowVoltageBattery_datatypes.h"
#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#include "hw_i2cs.h"
#endif

/**
 * @brief Initializes the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
ExitCode io_lowVoltageBattery_init();

/**
 * @brief OTP the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
ExitCode io_lowVoltageBattery_OTP(void);

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
double io_lowVoltageBattery_get_SOC();

/**
 * @brief Gets the battery voltage.
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
uint16_t io_lowVoltageBattery_get_voltage(voltage_cmd_t voltage_cell);

/**
 * @brief Handles releasing the semaphore after an interupt.
 *
 * @param GPIO_pin from the interupt.
 */
void io_lowVoltageBattery_completeAlert(uint16_t GPIO_pin);

/**
 * @brief Function to obtain the battery status from the chip
 *
 * @param bat_status The battery status returned from the chip
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowvoltageBattery_batteryStatus(Battery_Status *bat_status);

/**
 * @brief Function to obtain the battery status from the chip
 *
 * @param ctrl_status The control status returned from the chip
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowVoltageBattery_controlStatus(Control_Status *ctrl_status);