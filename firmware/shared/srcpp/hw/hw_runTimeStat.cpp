#include "hw_runTimeStat.hpp"
#include "hw_runTimeStat_helpers.h"
#include "FreeRTOSConfig.h"
#include <optional>
#include <functional>

namespace
{
std::optional<std::reference_wrapper<TIM_HandleTypeDef>> runTimeCounter           = std::nullopt;
unsigned long                                            ulHighFrequencyTimerTick = 0;
} // namespace

void hw::runtimeStat::init(TIM_HandleTypeDef &htim)
{
    runTimeCounter = htim;
}

void hw_runTimeStat_inc()
{
    ulHighFrequencyTimerTick++;
}

extern "C"
{
    inline void configureTimerForRunTimeStats()
    {
        assert(runTimeCounter.has_value());
        HAL_TIM_Base_Start_IT(&runTimeCounter.value().get());
    }
    inline unsigned long getRunTimeCounterValue()
    {
        return ulHighFrequencyTimerTick;
    }
}
