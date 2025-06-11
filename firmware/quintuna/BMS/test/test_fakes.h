#pragma once

#include <cstdint>

extern "C"
{
#include "io_irs.h"
#include "io_faultLatch.h"
}

namespace fakes
{
namespace irs
{
    void setNegativeState(IRsState state);
}
namespace tractiveSystem
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
} // namespace tractiveSystem

namespace faultLatches
{
    void resetFaultLatch(const FaultLatch *latch);
} // namespace faultLatches

namespace imd
{
    void setFrequency(float frequency);
    void setDutyCycle(float duty_cycle);
    void setPwmCounter(uint8_t counter);
} // namespace imd
} // namespace fakes
