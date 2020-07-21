#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize the secondary Scancon 2RMHF encoder
 * @param htim_secondary_encoder The timer handle for the secondary Scancon
 * 2RMHF encoder
 */
void Io_SecondaryScancon2RMHF_Init(TIM_HandleTypeDef *htim_secondary_encoder);

/**
 * Get the secondary Scancon 2RMHF encoder's counter value
 * @return The counter value for the secondary Scancon 2RMHF encoder
 */
uint32_t Io_SecondaryScancon2RMHF_GetEncoderCounter(void);

/**
 * Reset the secondary Scancon 2RMHF encoder's counter value to 0
 */
void Io_SecondaryScancon2RMHF_ResetEncoderCounter(void);
