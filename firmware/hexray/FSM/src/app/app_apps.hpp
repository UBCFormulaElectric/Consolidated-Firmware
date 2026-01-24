#pragma once
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