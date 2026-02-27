#include "hw_pwms.hpp"
#include "main.h"

// Note: A few PWM still need to be configured.
namespace hw::pwms
{
/* * IMD PWM Input
 * Pin: IMD_M_HS_3V3_Pin (PA8) -> TIM1_CH1
 * Mode: PWM Input on TI1
 * Trigger: TI1FP1 (Rising)
 */
constexpr PwmInput imd_pwm_input(
    htim1,
    HAL_TIM_ACTIVE_CHANNEL_1,
    TIMx_FREQUENCY / TIM1_PRESCALER,
    TIM_CHANNEL_1, // Rising Edge (Direct)
    TIM_CHANNEL_2, // Falling Edge (Indirect)
    TIM1_AUTO_RELOAD_REG);

/* * EVSE PWM Input
 * Pin: nEVSE_I_LIM_PWM_Pin (PA3) -> TIM5_CH4
 * Mode: PWM Input on TI4
 * Trigger: TI4FP4 (Rising)
 */
constexpr PwmInput evse_pwm_input(
    htim5,
    HAL_TIM_ACTIVE_CHANNEL_4,
    TIMx_FREQUENCY / TIM5_PRESCALER,
    TIM_CHANNEL_4, // Rising Edge (Direct)
    TIM_CHANNEL_3, // Falling Edge (Indirect)
    TIM5_AUTO_RELOAD_REG);
} // namespace hw::pwms

// HAL Interrupt Callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &hw::pwms::imd_pwm_input.get_timer_handle())
    {
        hw::pwms::imd_pwm_input.tick();
    }
    else if (htim == &hw::pwms::evse_pwm_input.get_timer_handle())
    {
        hw::pwms::evse_pwm_input.tick();
    }
}
