
#include "Io_TimerPwmGen.h"
#include "configs/App_ControlSystemConfig.h"
#include "math.h"

extern TIM_HandleTypeDef htim8;

void Io_TimerPwmGen_StartPwm(void)
{
    /* ADC is sampled before the inverter is enabled */

    uint32_t timer_cycles = 2 * __HAL_TIM_GET_AUTORELOAD(&htim8);

    // Set output compare registers before beginning PWM
    htim8.Instance->CCR1 = 0;
    htim8.Instance->CCR2 = 0;
    htim8.Instance->CCR3 = 0;
    htim8.Instance->CCR4 =
        (uint32_t)((float)timer_cycles / 2 - ((float)timer_cycles / 2) * 0.05f);
    htim8.Instance->CCR6 = (uint32_t)(((float)timer_cycles / 2) * 0.05f);

    // TODO do phase designations have to change?
    // Start 3-phase half bridge output compare
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_1);    // Phase A High
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_2);    // Phase B High
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_3);    // Phase C High
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_1); // Phase A Low
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_2); // Phase B Low
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_3); // Phase C Low
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_4);
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_6);

    // Start base counter
    HAL_TIM_Base_Start(&htim8);
}

void Io_TimerPwmGen_StopPwm(void)
{
    // Stop base counter
    HAL_TIM_Base_Stop(&htim8);

    // TODO do phase designations have to change?
    // Stop 3-phase half bridge output compare
    HAL_TIM_OC_Stop(&htim8, TIM_CHANNEL_1);    // Phase A High
    HAL_TIM_OC_Stop(&htim8, TIM_CHANNEL_2);    // Phase B High
    HAL_TIM_OC_Stop(&htim8, TIM_CHANNEL_3);    // Phase C High
    HAL_TIMEx_OCN_Stop(&htim8, TIM_CHANNEL_1); // Phase A Low
    HAL_TIMEx_OCN_Stop(&htim8, TIM_CHANNEL_2); // Phase B Low
    HAL_TIMEx_OCN_Stop(&htim8, TIM_CHANNEL_3); // Phase C Low
    HAL_TIM_OC_Stop(&htim8, TIM_CHANNEL_4);
    HAL_TIM_OC_Stop(&htim8, TIM_CHANNEL_6);
}

// Loads timer compare register value between 0 and the timer period*max
// modulation index
void Io_TimerPwmGen_LoadPwm(const struct PhaseValues *const phase_pwm_dur)
{
    uint32_t timer_cycles = 2 * __HAL_TIM_GET_AUTORELOAD(&htim8);

    // TODO double check that we're limiting these values to MAX_MOD_INDEX (0.9)
    //    if (phase_pwm_dur->a < MAX_MOD_INDEX && phase_pwm_dur->b <
    //    MAX_MOD_INDEX &&
    //        phase_pwm_dur->c < MAX_MOD_INDEX)
    //    {

    //    htim8.Instance->CCR1 = (uint32_t)((0.5f -
    //    (float)fabsf(phase_pwm_dur->a - 0.5f)) * (float)timer_cycles);
    //    htim8.Instance->CCR2 = (uint32_t)((0.5f -
    //    (float)fabsf(phase_pwm_dur->b - 0.5f)) * (float)timer_cycles);
    //    htim8.Instance->CCR3 = (uint32_t)((0.5f -
    //    (float)fabsf(phase_pwm_dur->c - 0.5f)) * (float)timer_cycles);

    htim8.Instance->CCR1 =
        (uint32_t)(phase_pwm_dur->a * (float)timer_cycles / 2);
    htim8.Instance->CCR2 =
        (uint32_t)(phase_pwm_dur->b * (float)timer_cycles / 2);
    htim8.Instance->CCR3 =
        (uint32_t)(phase_pwm_dur->c * (float)timer_cycles / 2);

    //    }
    //    else
    //    {
    //        // TODO Return exitcode here - mod index out of bounds. May need
    //        to
    //        // modify max mod index if nuissance tripping
    //    }
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

    uint32_t apb2_clock_freq;

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
    htim8.Init.Period = (uint32_t)(
        apb2_clock_freq / (2 * switching_freq * (htim8.Init.Prescaler + 1)) -
        1);
    HAL_TIM_Base_DeInit(&htim8);
    HAL_TIM_Base_Init(&htim8);
}

// Sets the dead time between high/low switching events, in nanoseconds.
void Io_TimerPwmGen_SetDeadTime(uint16_t dead_time)
{
    if (!(dead_time > MIN_DEAD_TIME && dead_time < MAX_DEAD_TIME))
    {
        // TODO Place exitcode here - dead time is out of bounds. May
        // need to modify min and max values
    }
    if (TIM8->BDTR & 0x300)
    {
        // TODO place exitcode here - deadtime bits are locked in BDTR register
    }

    double   t_dts;
    double   tck_int;
    uint32_t dead_time_reg; // TIM8_BDTR register is 32 bits wide

    // Read CKD bits of TIM8_CR1 register
    uint16_t ckd_bits = (TIM8->CR1 & TIM_CR1_CKD) >> 8;

    /* Get PCLK2 prescaler */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
    {
        /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
        tck_int = 1000000000.0 /
                  (double)HAL_RCC_GetPCLK2Freq(); // Clock period, in ns
    }
    else
    {
        /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
        tck_int = 1000000000.0 / (double)HAL_RCC_GetPCLK2Freq() /
                  2; // Clock period, in ns
    }

    if (ckd_bits == 0)
    {
        t_dts = tck_int;
    }
    if (ckd_bits == 0x1)
    {
        t_dts = 2 * tck_int;
    }
    if (ckd_bits == 0x2)
    {
        t_dts = 4 * tck_int;
    }

    // Read DTG bits of TIM8_BDTR
    if (dead_time < 127 * tck_int)
    {
        // DTG[7:5]=0xx -> dead_time = DTG[7:0]*tdtg with tdtg = tDTS.
        dead_time_reg = (uint32_t)((double)dead_time / t_dts);
        TIM8->BDTR |= dead_time_reg & 0xFF;
    }
    else if (dead_time < (64 + 63) * tck_int * 2)
    {
        // DTG[7:5]=10x -> dead_time = (64+DTG[5:0])*tdtg with Tdtg = 2 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (2.0 * t_dts) - 64.0) |
                        TIM_BDTR_DTG_7;
        TIM8->BDTR |= dead_time_reg;
    }
    else if (dead_time < (32 + 31) * tck_int * 8)
    {
        // DTG[7:5]=110 -> dead_time = (32+DTG[4:0])*tdtg with Tdtg = 8 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (8.0 * t_dts) - 32.0) |
                        TIM_BDTR_DTG_7 | TIM_BDTR_DTG_6;
        TIM8->BDTR |= dead_time_reg;
    }
    else if (dead_time < (32 + 31) * tck_int * 16)
    {
        // DTG[7:5]=111 -> dead_time = (32+DTG[4:0])*tdtg with Tdtg = 16 * tDTS
        dead_time_reg = (uint32_t)((double)dead_time / (16.0 * t_dts) - 32.0) |
                        TIM_BDTR_DTG_7 | TIM_BDTR_DTG_6 | TIM_BDTR_DTG_5;
        TIM8->BDTR |= dead_time_reg;
    }
    else
    {
        // Set BDTR to a default value of 1000ns
        // DTG[7:5]=10x -> dead_time = (64+DTG[5:0])*tdtg with Tdtg = 2 * tDTS
        dead_time_reg =
            (uint32_t)((double)1000 / (2.0 * t_dts) - 64.0) | TIM_BDTR_DTG_7;
        TIM8->BDTR |= dead_time_reg;
        // TODO exitcode error here, BDTR register set improperly
    }
    // TODO return exitcode success here
}
