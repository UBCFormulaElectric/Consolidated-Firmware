#include <assert.h>
#include <stdlib.h>
#include "Io_PrimaryScancon2RMHF.h"

// TODO: Test the following functions with the primary Scancon 2RMHF encoder
// #667

static TIM_HandleTypeDef *primary_encoder_htim;

void Io_PrimaryScancon2RMHF_Init(TIM_HandleTypeDef *const htim_primary_encoder)
{
    assert(htim_primary_encoder != NULL);

    primary_encoder_htim = htim_primary_encoder;
    assert(HAL_TIM_Encoder_Start(primary_encoder_htim, TIM_CHANNEL_ALL) == HAL_OK);
}

uint32_t Io_PrimaryScancon2RMHF_GetEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(primary_encoder_htim);
}

void Io_PrimaryScancon2RMHF_ResetEncoderCounter(void)
{
    __HAL_TIM_SET_COUNTER(primary_encoder_htim, 0U);
}
