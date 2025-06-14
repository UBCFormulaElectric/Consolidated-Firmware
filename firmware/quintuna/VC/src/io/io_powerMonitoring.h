#pragma once
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize power monitoring chip.
 *
 * @return true if successfully initalized, false otherwise.
 */
bool io_powerMonitoring_init(void);

/**
 * @brief Refresh the chip to get updated values, with a 1ms delay.
 */
void io_powerMonitoring_refresh(void);

/**
 * @brief Read bus voltage on channel ch (1 or 2).
 *
 * @param ch channel to read
 * @param voltage read voltage from chip
 */
void io_powerMonitoring_read_voltage(uint8_t ch, float *voltage);

/**
 * @brief Read bus voltage on channel ch (1 or 2).
 *
 * @param ch channel to read
 * @param voltage read voltage from chip
 */
void io_powerMonitoring_read_current(uint8_t ch, float *current);

/**
 * @brief Read bus power on channel ch (1 or 2).
 *
 * @param ch
 * @param power read power from chip on
 */
void io_powerMonitoring_read_power(uint8_t ch, float *power);
