#pragma once

#define DISAGREEMENT_TIME_TO_FAULT (10U)
#define DISAGREEMENT_TIME_TO_CLEAR (10U)
#define OCSC_TIME_TO_FAULT (10U)
#define OCSC_TIME_TO_CLEAR (10U)
namespace app::apps
{
/**
 * Initialize apps internal signals.
 */
void init(void);

/**
 * Broadcast all apps alerts/faults/messages.
 */
void broadcast(void);
} // namespace app::apps