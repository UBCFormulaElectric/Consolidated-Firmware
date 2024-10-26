#include "hw_pwms.h"
#include "main.h"

namespace hw::pwm
{
    const PwmInputFreqOnly coolant_config { &htim3, 
                                            TIMx_FREQUENCY / TIM12_PRESCALER,
                                            TIM_CHANNEL_1, 
                                            TIM12_AUTO_RELOAD_REG,
                                            HAL_TIM_ACTIVE_CHANNEL_1}
                                            
} // namespace hw::pwm
