#include "test_fakes.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <cmath>

#include "io_canTx.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"
#include "util_utils.hpp"

namespace io
{
namespace irs
{
    static auto positive_state  = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    static auto precharge_state = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    static auto negative_state  = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;

    app::can_utils::ContactorState negativeState()
    {
        return negative_state;
    }
    void setPositive(const app::can_utils::ContactorState state)
    {
        positive_state = state;
    }
    app::can_utils::ContactorState positiveState()
    {
        return positive_state;
    }
    void setPrecharge(const app::can_utils::ContactorState state)
    {
        precharge_state = state;
    }
    app::can_utils::ContactorState prechargeState()
    {
        return precharge_state;
    }
} // namespace irs

namespace imd
{
    static float   frequency   = 0.0f;
    static float   duty_cycle  = 0.0f;
    static uint8_t pwm_counter = 0;
    float          getFrequency()
    {
        return frequency;
    }
    float getDutyCycle()
    {
        return duty_cycle;
    }
    uint8_t pwmCounterTick()
    {
        return pwm_counter;
    }
    uint32_t getTimeSincePowerOn()
    {
        return time::getCurrentMs();
    }
} // namespace imd
} // namespace io

namespace fakes
{
namespace irs
{
    void setNegativeState(const app::can_utils::ContactorState state)
    {
        io::irs::negative_state = state;
    }
} // namespace irs

namespace imd

{
    void setFrequency(const float frequency)
    {
        io::imd::frequency = frequency;
    }
    void setDutyCycle(const float duty_cycle)
    {
        io::imd::duty_cycle = duty_cycle;
    }
    void setPwmCounter(const uint8_t counter)
    {
        io::imd::pwm_counter = counter;
    }
} // namespace imd
} // namespace fakes
