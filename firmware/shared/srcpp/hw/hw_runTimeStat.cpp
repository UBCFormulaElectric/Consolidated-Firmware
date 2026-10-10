#include "hw_runTimeStat.hpp"
#include "hw_hal.hpp"

void hw::runtimeStat::init([[maybe_unused]] TIM_HandleTypeDef &htim)
{
    // kept backward compatibility with board startup calls.
    // hardware timer interrupts are not required cuz DWT cycle counter exists
}

void hw::runtimeStat::inc()
{
    // kept for backward compatibility with board timer callbacks
}

extern "C"
{
    void configureTimerForRunTimeStats()
    {
#if defined(CoreDebug) && defined(DWT)
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if defined(__CORTEX_M) && (__CORTEX_M == 7U)
        DWT->LAR = 0xC5ACCE55;
#endif
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#endif
    }

    unsigned long getRunTimeCounterValue()
    {
#if defined(DWT) && defined(CoreDebug)
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0 ||
        (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0) // ts exists to force enable dwt if j-link diables it
    {
        // re-enable
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        #if defined(__CORTEX_M) && (__CORTEX_M == 7U)
        DWT->LAR = 0xC5ACCE55;
        #endif
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    } // Can be removed if we have it such j-link does not disalbe global trace block
        return DWT->CYCCNT >> 8;
#else
        return 0;
#endif
    }
}
