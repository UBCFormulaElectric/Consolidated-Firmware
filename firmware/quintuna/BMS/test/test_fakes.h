#pragma once

extern "C"
{
#include "io_irs.h"
}

namespace fakes
{
namespace irs
{
    void setNegativeState(ContactorsState closed);
}
namespace tractiveSystem
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
} // namespace tractiveSystem
} // namespace fakes
