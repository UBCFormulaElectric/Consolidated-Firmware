#include "hw_runTimeStat.hpp"
#include "hw_hal.hpp"

void hw::runtimeStat::init([[maybe_unused]] TIM_HandleTypeDef &htim)
{
    // kept backward compatibility with board startup calls.
    // hardware timer interrupts are not required cuz DWT cycle counter exists
}

void hw::runtimeStat::inc()
{
    // kept for backward compatibility with board timer callbacks.
}

extern "C"
{
    void configureTimerForRunTimeStats()
    {
#if defined(CoreDebug) && defined(DWT)
        // DEMCR.TRCENA is the global enable for the trace blocks (DWT, ITM). The DWT does nothing
        // until this is set, so it has to come first
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if defined(__CORTEX_M) && (__CORTEX_M == 7U)
        // The Cortex-M7 DWT is software locked out of reset and ignores writes until this
        // CoreSight key is written to its Lock Access Register
        DWT->LAR = 0xC5ACCE55;
#endif
        // DWT_CTRL.CYCCNTENA starts CYCCNT, a free-running 32-bit counter of core clock cycles.
        // FreeRTOS uses it as the time base for per-task runtime
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#endif
    }

    unsigned long getRunTimeCounterValue()
    {
#if defined(DWT) && defined(CoreDebug)
        if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0 ||
            (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0) // re-configure if global trace block disabled
        {
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if defined(__CORTEX_M) && (__CORTEX_M == 7U)
            DWT->LAR = 0xC5ACCE55;
#endif
            DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        }
        return DWT->CYCCNT;
#else
        return 0;
#endif
    }
}
