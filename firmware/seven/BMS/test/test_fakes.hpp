#pragma once

#include <cstdint>
#include <array>

#include "app_canUtils.hpp"
#include "app_canTx.hpp"

namespace fakes
{
namespace irs
{
    void setNegativeState(app::can_utils::ContactorState state);
}

namespace imd
{
    void setFrequency(float frequency);
    void setDutyCycle(float duty_cycle);
    void setPwmCounter(uint8_t counter);
} // namespace imd
} // namespace fakes
