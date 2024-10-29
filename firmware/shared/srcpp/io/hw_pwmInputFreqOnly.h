#pragma once

#include "hw_hal.h"
#include <stdbool.h>

namespace hw
{
class PwmInputFreqOnly
{
  private:
    TIM_HandleTypeDef *const    htim;
    const float                 tim_frequency_hz;
    const uint32_t              tim_channel;
    const uint32_t              tim_auto_reload_reg;
    const HAL_TIM_ActiveChannel tim_active_channel;

    float frequency_hz = 0.0f;

    bool     first_tick       = true;
    uint32_t curr_rising_edge = 0U;
    uint32_t prev_rising_edge = 0U;

    size_t tim_overflow_count = 0U;

    void setFrequency(float frequency);

  public:
    explicit PwmInputFreqOnly(
        TIM_HandleTypeDef *const                              htim_in,
        float                                                 tim_frequency_hz_in,
        uint32_t                                              tim_channel_in,
        uint32_t tim_auto_reload_reg_in HAL_TIM_ActiveChannel tim_active_channel_in)
      : htim(htim_in),
        tim_frequency_hz(tim_frequency_hz_in),
        tim_channel(tim_channel_in),
        tim_auto_reload_reg(tim_auto_reload_reg_in),
        tim_active_channel(tim_active_channel_in)
    {
    }

    void init(void);

    float getFrequency(void);

    TIM_HandleTypeDef *getTimerHandle(void);

    HAL_TIM_ActiveChannel getTimerActiveChannel(void);

    void tick(void);

    void checkIfPwmIsActive(void);
};
} // namespace hw
