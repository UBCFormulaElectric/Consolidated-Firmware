#pragma once

#include <stm32f3xx_hal.h>

void Io_Scancon2RMHF_Init(
    TIM_HandleTypeDef *htim_primary_quad_encoder,
    TIM_HandleTypeDef *htim_secondary_quad_encoder);

uint32_t Io_Scancon2RMHF_GetPrimaryEncoderCounter(void);

uint32_t Io_Scancon2RMHF_GetSecondaryEncoderCounter(void);
