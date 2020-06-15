#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initializes both the primary and secondary Scancon 2RMHF encoders
 * @param htim_primary_encoder The timer handle for the primary encoder
 * @param htim_secondary_encoder The timer handle for the secondary encoder
 */
void Io_Scancon2RMHF_Init(
    TIM_HandleTypeDef *htim_primary_encoder,
    TIM_HandleTypeDef *htim_secondary_encoder);

/**
 * Get the primary encoder's counter value
 * @return The counter value for the primary encoder
 */
uint32_t Io_Scancon2RMHF_GetPrimaryEncoderCounter(void);

/**
 * Get the secondary encoder's counter value
 * @return The counter value for the secondary encoder
 */
uint32_t Io_Scancon2RMHF_GetSecondaryEncoderCounter(void);

/**
 * Reset the primary encoder's counter value
 */
void Io_Scancon2RMHF_ResetPrimaryEncoderCounter(void);

/**
 * Reset the secondary encoder's counter value
 */
void Io_Scancon2RMHF_ResetSecondaryEncoderCounter(void);
