#pragma once

#include "app_canUtils.hpp"

namespace fakes
{
namespace irs
{
    void setNegativeState(app::can_utils::ContactorState state);
}
namespace ts
{
    void setTsVoltage(float tsVoltage);
}
namespace segments
{
    void setPackVoltage(result<float> packVoltage);
}
}