#include <assert.h>
#include <stdlib.h>
#include "Io_Scancon2RMHF.h"

static TIM_HandleTypeDef *primary_quad_encoder, *secondary_quad_encoder;

void Io_Scancon2RMHF_Init(
    TIM_HandleTypeDef *const htim_primary_quad_encoder,
    TIM_HandleTypeDef *const htim_secondary_quad_encoder)
{
    assert(htim_primary_quad_encoder != NULL);
    assert(htim_secondary_quad_encoder != NULL);

    primary_quad_encoder   = htim_primary_quad_encoder;
    secondary_quad_encoder = htim_secondary_quad_encoder;

    assert(
        HAL_TIM_Encoder_Start(htim_primary_quad_encoder, TIM_CHANNEL_ALL) !=
        HAL_OK);
    assert(
        HAL_TIM_Encoder_Start(htim_secondary_quad_encoder, TIM_CHANNEL_ALL) !=
        HAL_OK);
}

uint32_t Io_Scancon2RMHF_GetPrimaryEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(primary_quad_encoder);
}

uint32_t Io_Scancon2RMHF_GetSecondaryEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(secondary_quad_encoder);
}
