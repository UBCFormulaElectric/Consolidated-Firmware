//-- Includes ------------------------------------------------------------------
#include "stm32f4xx_it.h"

//-- Cortex-M4 Processor Exceptions Handlers -----------------------------------
void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1) {
    }
}

void MemManage_Handler(void)
{
    while (1) {
    }
}

void BusFault_Handler(void)
{
    while (1) {
    }
}

void UsageFault_Handler(void)
{
    while (1) {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

//-- STM32F4xx Peripherals Interrupt Handlers ----------------------------------
// TIM2 update interrupt
void TIM2_IRQHandler(void)
{
    // Check whether update interrupt is pending
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
        // Clear the update interrupt flag
        LL_TIM_ClearFlag_UPDATE(TIM2);
    }

    // TIM2 update interrupt processing
    TimerUpdate_Callback();
}
