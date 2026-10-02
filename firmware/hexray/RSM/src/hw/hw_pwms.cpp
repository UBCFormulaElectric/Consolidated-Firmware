#include "hw_pwms.hpp"
#include "main.h"

// constexpr uint32_t TIMx_FREQUENCY = 64000000;
// constexpr uint32_t TIM1_FREQUENCY = 100;
// constexpr uint16_t TIM1_PRESCALER = 64;
// constexpr uint16_t TIM1_ARR = 1000;
constexpr bool TIMER_RESET_MODE  = false;
constexpr bool MIN_EXPECTED_FREQ = 10U;

constexpr hw::PwmInput flow_meter_config{
    htim1,    HAL_TIM_ACTIVE_CHANNEL_1, TIM1_FREQUENCY,   TIM_CHANNEL_1, TIM_CHANNEL_2,
    TIM1_ARR, TIMER_RESET_MODE,         MIN_EXPECTED_FREQ
};

extern "C"
{
    void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
    {
        assert(htim == &flow_meter_config.get_timer_handle());
        flow_meter_config.tick();
    }
}