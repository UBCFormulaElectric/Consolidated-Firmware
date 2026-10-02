#include "hw_pwms.hpp"
#include "main.h"

constexpr bool     TIM1_RESET_MODE           = true;  // TIM1 counter resets on every rising edge
constexpr bool     TIM5_RESET_MODE           = false; // TIM5 counter does not reset on rising edge
constexpr uint32_t TIM1_ACTIVE_ARR_THRESHOLD = 2U;
constexpr uint32_t TIM5_ACTIVE_ARR_THRESHOLD = 4U;

/* * IMD PWM Input
 * Pin: IMD_M_HS_3V3_Pin (PA8) -> TIM1_CH1
 * Mode: PWM Input on TI1
 * Trigger: TI1FP1 (Rising)
 */
constexpr hw::PwmInput imd_pwm_input(
    htim1,
    HAL_TIM_ACTIVE_CHANNEL_1,
    (TIMx_FREQUENCY / (TIM1_PRESCALER + 1U)),
    TIM_CHANNEL_1, // Rising Edge (Direct)
    TIM_CHANNEL_2, // Falling Edge (Indirect)
    TIM1_AUTO_RELOAD_REG,
    TIM1_RESET_MODE,
    TIM1_PWM_MIN_FREQUENCY,
    TIM1_IC_PRESCALER, // CAUTION: Two sources of truth, one user constant in cubemx and one setting in cubemx.
    TIM1_ACTIVE_ARR_THRESHOLD);

/* * EVSE PWM Input
 * Pin: nEVSE_I_LIM_PWM_Pin (PA3) -> TIM5_CH4
 * Mode: PWM Input on TI4
 */
constexpr hw::PwmInput evse_pwm_input(
    htim5,
    HAL_TIM_ACTIVE_CHANNEL_4,
    (TIMx_FREQUENCY / (TIM5_PRESCALER + 1U)),
    TIM_CHANNEL_4, // Rising Edge (Direct)
    TIM_CHANNEL_3, // Falling Edge (Indirect)
    TIM5_AUTO_RELOAD_REG,
    TIM5_RESET_MODE,
    TIM5_PWM_MIN_FREQUENCY,
    TIM5_IC_PRESCALER, // CAUTION: Two sources of truth, one user constant in cubemx and one setting in cubemx.
    TIM5_ACTIVE_ARR_THRESHOLD);

// HAL Interrupt Callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &imd_pwm_input.get_timer_handle())
    {
        imd_pwm_input.tick();
    }
    else if (htim == &evse_pwm_input.get_timer_handle())
    {
        evse_pwm_input.tick();
    }
}
