#pragma once

#include "hw_hal.h"

/**
 * Initialize the secondary Scancon 2RMHF encoder
 * @param htim The timer handle for the secondary Scancon 2RMHF encoder
 */
void Io_SecondaryScancon2RMHF_Init(TIM_HandleTypeDef *htim);

/**
 * Get the secondary Scancon 2RMHF's counter value
 * @return The counter value for the secondary Scancon 2RMHF encoder
 */
uint32_t Io_SecondaryScancon2RMHF_GetEncoderCounter(void);

/**
 * Reset the secondary Scancon 2RMHF's counter value to 0
 */
void Io_SecondaryScancon2RMHF_SetEncoderCounter(uint32_t counter_value);
