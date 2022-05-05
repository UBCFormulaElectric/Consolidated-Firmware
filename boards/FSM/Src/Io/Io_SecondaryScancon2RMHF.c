#include <assert.h>
#include <stdlib.h>
#include "Io_SecondaryScancon2RMHF.h"

// TODO: Test the following functions with the secondary Scancon 2RMHF encoder
// #667

static TIM_HandleTypeDef *secondary_encoder_htim;

void Io_SecondaryScancon2RMHF_Init(TIM_HandleTypeDef *const htim)
{
    assert(htim != NULL);

    secondary_encoder_htim = htim;
    assert(HAL_TIM_Encoder_Start(secondary_encoder_htim, TIM_CHANNEL_ALL) == HAL_OK);
}

uint32_t Io_SecondaryScancon2RMHF_GetEncoderCounter(void)
{
    return __HAL_TIM_GET_COUNTER(secondary_encoder_htim);
}

void Io_SecondaryScancon2RMHF_ResetEncoderCounter(void)
{
    __HAL_TIM_SET_COUNTER(secondary_encoder_htim, 0U);
}
