#pragma once

#include "hw_hal.h"

/**
 * Initialize the primary Scancon 2RMHF encoder
 * @param htim The timer handle for the primary Scancon 2RMHF
 * encoder
 */
void Io_PrimaryScancon2RMHF_Init(TIM_HandleTypeDef *htim);

/**
 * Get the primary Scancon 2RMHF's counter value
 * @return The counter value for the primary Scancon 2RMHF encoder
 */
uint32_t Io_PrimaryScancon2RMHF_GetEncoderCounter(void);

/**
 * Reset the primary Scancon 2RMHF's counter value to 0
 */
void Io_PrimaryScancon2RMHF_SetEncoderCounter(uint32_t counter_value);
