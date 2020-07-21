#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize the primary Scancon 2RMHF encoder
 * @param htim_primary_encoder The timer handle for the primary Scancon 2RMHF
 * encoder
 */
void Io_PrimaryScancon2RMHF_Init(TIM_HandleTypeDef *htim_primary_encoder);

/**
 * Get the primary Scancon 2RMHF encoder's counter value
 * @return The counter value for the primary Scancon 2RMHF encoder
 */
uint32_t Io_PrimaryScancon2RMHF_GetEncoderCounter(void);

/**
 * Reset the Primary Scancon 2RMHF encoder's counter value to 0
 */
void Io_PrimaryScancon2RMHF_ResetEncoderCounter(void);
