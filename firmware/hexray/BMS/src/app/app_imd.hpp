#pragma once
#include <optional>
#include "app_canUtils.hpp"

namespace app::imd
{
struct Condition
{
    app::can_utils::ImdConditionName           name;
    bool                                       valid_duty_cycle;
    std::optional<uint16_t>                    insulation_measurement_dcp_kohms;
    std::optional<app::can_utils::ImdSstState> speed_start_status;
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