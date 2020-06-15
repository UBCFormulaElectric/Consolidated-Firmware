#include <assert.h>
#include <stdlib.h>
#include "Io_Scancon2RMHF.h"

struct Encoder
{
    TIM_HandleTypeDef *htim;
};

static struct Encoder primary_encoder, secondary_encoder;

void Io_Scancon2RMHF_Init(
    TIM_HandleTypeDef *const htim_primary_encoder,
    TIM_HandleTypeDef *const htim_secondary_encoder)
{
    assert(htim_primary_encoder != NULL);
    assert(htim_secondary_encoder != NULL);

    primary_encoder.htim   = htim_primary_encoder;
    secondary_encoder.htim = htim_secondary_encoder;

    assert(
        HAL_TIM_Encoder_Start(primary_encoder.htim, TIM_CHANNEL_ALL) != HAL_OK);
    assert(
        HAL_TIM_Encoder_Start(htim_secondary_encoder, TIM_CHANNEL_ALL) !=
        HAL_OK);
}

uint32_t Io_Scancon2RMHF_GetPrimaryEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(primary_encoder.htim);
}

uint32_t Io_Scancon2RMHF_GetSecondaryEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(secondary_encoder.htim);
}

void Io_Scancon2RMHF_ResetPrimaryEncoderCounter(void)
{
    __HAL_TIM_SET_COUNTER(primary_encoder.htim, 0U);
}

void Io_Scancon2RMHF_ResetSecondaryEncoderCounter(void)
{
    __HAL_TIM_SET_COUNTER(secondary_encoder.htim, 0U);
}
