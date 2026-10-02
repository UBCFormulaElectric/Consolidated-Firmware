#include "hw_runTimeStat.hpp"
#include <cassert>
#include <optional>
#include <functional>

namespace
{
unsigned long                                            ulHighFrequencyTimerTick = 0;
std::optional<std::reference_wrapper<TIM_HandleTypeDef>> runTimeCounter           = std::nullopt;
} // namespace

void hw::runtimeStat::init(TIM_HandleTypeDef &htim)
{
    runTimeCounter = htim;
}

void hw::runtimeStat::inc()
{
    ulHighFrequencyTimerTick++;
}

void configureTimerForRunTimeStats()
{
    assert(runTimeCounter.has_value());
    HAL_TIM_Base_Start_IT(&runTimeCounter.value().get());
}
unsigned long getRunTimeCounterValue()
{
    return ulHighFrequencyTimerTick;
}