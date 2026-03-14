#pragma once
#include "app_canUtils.hpp"

namespace app::imd
{

enum class Sst
{
    GOOD,
    BAD,
    INVALID
};

struct Condition
{
    app::can_utils::ImdConditionName name;
    bool                             valid_duty_cycle;
    union Payload
    {
        uint16_t insulation_measurement_dcp_kohms;
        Sst      speed_start_status;
    } payload;
};

/**
 * Get the condition for the given IMD
 * @param imd The IMD to get condition for
 * @return The condition for the given IMD
 */
Condition getCondition(void);

/**
 * Broadcast state of the IMD over the CAN bus.
 */
void broadcast(void);
} // namespace app::imd