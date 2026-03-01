#pragma once
#include "app_canUtils.hpp"

enum class ImdSst
{
    GOOD,
    BAD,
    INVALID
};

struct ImdCondition
{
    app::can_utils::ImdConditionName name;
    bool                             valid_duty_cycle;
    union Payload
    {
        uint16_t insulation_measurement_dcp_kohms;
        ImdSst   speed_start_status;
    } payload;
};

namespace app::imd
{
/**
 * Get the condition for the given IMD
 * @param imd The IMD to get condition for
 * @return The condition for the given IMD
 */
ImdCondition getCondition(void);

/**
 * Broadcast state of the IMD over the CAN bus.
 */
void broadcast(void);
} // namespace app::imd