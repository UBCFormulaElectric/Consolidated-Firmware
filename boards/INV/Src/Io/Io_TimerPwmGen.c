
#include "Io_TimerPwmGen.h"
#include "configs/App_ControlSystemConfig.h"

TIM_HandleTypeDef *timer_pwm_gen_handle;

void Io_TimerPwmGen_Init(TIM_HandleTypeDef *const tim_handle)
{
    timer_pwm_gen_handle = tim_handle;
}

void Io_TimerPwmGen_StartPwm(void)
{
    if (timer_pwm_gen_handle == NULL)
    {
        // TODO Return ExitCode here
    }

    // Start base counter
    HAL_TIM_Base_Start_IT(timer_pwm_gen_handle);
    uint32_t timer_period = timer_pwm_gen_handle->Init.Period;

    /* ADC is sampled before the inverter is enabled */

    // Set output compare registers before beginning PWM
    timer_pwm_gen_handle->Instance->CCR1 = 0;
    timer_pwm_gen_handle->Instance->CCR2 = 0;
    timer_pwm_gen_handle->Instance->CCR3 = 0;

    // ADC is sampled 300 cycles before control loop calculations begin
    timer_pwm_gen_handle->Instance->CCR4 = timer_period - 300;
    timer_pwm_gen_handle->Instance->CCR6 = timer_period / 2 - 300;

    // TODO do phase designations have to change?
    // Start 3-phase half bridge output compare
    HAL_TIM_OC_Start(timer_pwm_gen_handle, TIM_CHANNEL_1);    // Phase A High
    HAL_TIM_OC_Start(timer_pwm_gen_handle, TIM_CHANNEL_2);    // Phase B High
    HAL_TIM_OC_Start(timer_pwm_gen_handle, TIM_CHANNEL_3);    // Phase C High
    HAL_TIMEx_OCN_Start(timer_pwm_gen_handle, TIM_CHANNEL_1); // Phase A Low
    HAL_TIMEx_OCN_Start(timer_pwm_gen_handle, TIM_CHANNEL_2); // Phase B Low
    HAL_TIMEx_OCN_Start(timer_pwm_gen_handle, TIM_CHANNEL_3); // Phase C Low
    HAL_TIMEx_OCN_Start(timer_pwm_gen_handle, TIM_CHANNEL_4); // ADC TRGO2 Event
    HAL_TIMEx_OCN_Start(timer_pwm_gen_handle, TIM_CHANNEL_6); // ADC TRGO2 Event
}

void Io_TimerPwmGen_StopPwm(void)
{
    if (timer_pwm_gen_handle == NULL)
    {
        // TODO Return ExitCode here
    }

    // Stop base counter
    HAL_TIM_Base_Stop_IT(timer_pwm_gen_handle);

    // TODO do phase designations have to change?
    // Stop 3-phase half bridge output compare
    HAL_TIM_OC_Stop(timer_pwm_gen_handle, TIM_CHANNEL_1);    // Phase A High
    HAL_TIM_OC_Stop(timer_pwm_gen_handle, TIM_CHANNEL_2);    // Phase B High
    HAL_TIM_OC_Stop(timer_pwm_gen_handle, TIM_CHANNEL_3);    // Phase C High
    HAL_TIMEx_OCN_Stop(timer_pwm_gen_handle, TIM_CHANNEL_1); // Phase A Low
    HAL_TIMEx_OCN_Stop(timer_pwm_gen_handle, TIM_CHANNEL_2); // Phase B Low
    HAL_TIMEx_OCN_Stop(timer_pwm_gen_handle, TIM_CHANNEL_3); // Phase C Low
    HAL_TIMEx_OCN_Stop(timer_pwm_gen_handle, TIM_CHANNEL_4); // ADC TRGO2 Event
    HAL_TIMEx_OCN_Stop(timer_pwm_gen_handle, TIM_CHANNEL_6); // ADC TRGO2 Event
}

// Loads timer compare register value between 0 and the timer period*max
// modulation index
void Io_TimerPwmGen_LoadPwm(const struct PhaseValues *const phase_pwm_dur)
{
    if (timer_pwm_gen_handle == NULL)
    {
        // TODO Return ExitCode here - null timer handle
    }

    uint32_t timer_period = timer_pwm_gen_handle->Init.Period;

    //TODO double check that we're limiting these values to MAX_MOD_INDEX (0.9)
    if (phase_pwm_dur->a < MAX_MOD_INDEX &&
        phase_pwm_dur->b < MAX_MOD_INDEX &&
        phase_pwm_dur->c < MAX_MOD_INDEX)
    {
        timer_pwm_gen_handle->Instance->CCR1 =
            (uint32_t)(phase_pwm_dur->a*timer_period / 2);
        timer_pwm_gen_handle->Instance->CCR2 =
            (uint32_t)(phase_pwm_dur->b*timer_period / 2);
        timer_pwm_gen_handle->Instance->CCR3 =
            (uint32_t)(phase_pwm_dur->c*timer_period / 2);
    }
    else
    {
        // TODO Return exitcode here - mod index out of bounds. May need to
        // modify max mod index if nuissance tripping
    }
    // TODO return exitcode ok
}

void Io_TimerPwmGen_SetSwitchingFreq(const uint16_t switching_freq)
{
    if (!(switching_freq > MIN_SWITCHING_FREQUENCY &&
          switching_freq < MAX_SWITCHING_FREQUENCY))
    {
        // TODO Place exitcode here - switching frequency is out of bounds. May
        // need to modify min and max values
    }

    uint32_t apb2_clock_freq = 2 * HAL_RCC_GetPCLK2Freq();

    //  TODO use this section of code to find out prescalar value. in our case,
    //  we know it's 2x
    /* Get PCLK2 frequency */
    uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();

    /* Get PCLK2 prescaler */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
    {
        /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
        apb2_clock_freq = pclk2;
    }
    else
    {
        /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
        apb2_clock_freq = 2 * pclk2;
    }

    /* Change switching frequency by modifying the tim8 period */
    timer_pwm_gen_handle->Init.Period = (uint16_t)(
        apb2_clock_freq /
            (2 * switching_freq * (timer_pwm_gen_handle->Init.Prescaler + 1)) -
        1);
}

// Sets the dead time between high/low switching events, in nanoseconds.
void Io_TimerPwmGen_SetDeadTime(uint16_t dead_time)
{
    if (!(dead_time > MIN_DEAD_TIME && dead_time < MAX_DEAD_TIME))
    {
        // TODO Place exitcode here - switching frequency is out of bounds. May
        // need to modify min and max values
    }
    if (TIM8->BDTR & 0x300)
    {
        // TODO place exitcode here - deadtime bits are locked in BDTR register
    }

    uint32_t dead_time_reg; // TIM8_BDTR register is 32 bits wide

    // Read CKD bits of TIM8_CR1 register
    uint16_t ckd_bits = (TIM8->CR1 & 0x180) >> 8;
    double   tck_int =
        1000000000.0 / (double)HAL_RCC_GetPCLK2Freq(); // Clock period, in ns
    double t_dts = 2 * ckd_bits * tck_int;

    // Read DTG bits of TIM8_BDTR
    if (!(TIM8->BDTR & 0x80) >> 7)
    {
        // DTG[7:5]=0xx -> dead_time = DTG[7:0]*tdtg with tdtg = tDTS.
        dead_time_reg = (uint32_t)((double)dead_time / t_dts);
        TIM8->BDTR |= dead_time_reg & 0xFF;
    }
    else if ((TIM8->BDTR & 0xC0) >> 6 == 2)
    {
        // DTG[7:5]=10x -> dead_time = (64+DTG[5:0])*tdtg with Tdtg = 2 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (2.0 * t_dts) - 64.0);
        TIM8->BDTR |= dead_time_reg & 0x3F;
    }
    else if ((TIM8->BDTR & 0xE0) >> 5 == 6)
    {
        // DTG[7:5]=110 -> dead_time = (32+DTG[4:0])*tdtg with Tdtg = 8 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (8.0 * t_dts) - 32.0);
        TIM8->BDTR |= dead_time_reg & 0x1F;
    }
    else if ((TIM8->BDTR & 0xE0) >> 5 == 7)
    {
        // DTG[7:5]=111 -> dead_time = (32+DTG[4:0])*tdtg with Tdtg = 16 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (16.0 * t_dts) - 32.0);
        TIM8->BDTR |= dead_time_reg & 0x1F;
    }
    else
    {
        // TODO exitcode error here, BDTR register set improperly
    }
    // TODO return exitcode success here
}
