#pragma once

extern "C"
{
#include "io_irs.h"
#include "io_faultLatch.h"
}

namespace fakes
{
namespace irs
{
    void setNegativeState(ContactorsState state);
}
namespace tractiveSystem
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
} // namespace tractiveSystem

namespace faultLatch
{
    void resetFaultLatch(const FaultLatch *latch);
} // namespace faultLatch
} // namespace fakes
