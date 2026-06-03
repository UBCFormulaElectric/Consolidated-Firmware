#pragma once
#include <expected>
#include <cfloat>
#include <limits>
#include "util_errorCodes.hpp"

enum class BalancingState
{
    DISABLED,
    SETTLE,
    BALANCE
};
enum class BalancingPhase
{
    ODD,
    EVEN
};
enum class ChargeState
{
    STOPPED,
    CHARGING
};
struct MinCell
{
    uint8_t index;
    float   value;
};
struct MaxCell
{
    uint8_t index;
    float   value;
};
namespace app::batteryMonitoring
{
result<void> init();
result<void> update();
} // namespace app::batteryMonitoring
