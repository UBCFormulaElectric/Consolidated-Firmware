#pragma once

#include <stdint.h>

struct Clock;

/**
 * Allocate and initialize a clock
 * @return The created clock, whose ownership is transferred to the caller
 */
struct Clock *App_SharedClock_Create(void);

/**
 * Deallocate the memory used by the given clock
 * @param clock The clock to deallocate
 */
void App_SharedClock_Destroy(struct Clock *clock);

/**
 * Set the current time for the given clock, in milliseconds
 * @param clock The clock to set current time for
 * @param current_time_ms The current time to set, in milliseconds
 */
void App_SharedClock_SetCurrentTimeInMilliseconds(struct Clock *clock, uint32_t current_time_ms);

/**
 * Set the previous time for the given clock, in milliseconds
 * @param clock The clock to set the previous time for
 * @param current_time_ms The previous time to set, in milliseconds
 */
void App_SharedClock_SetPreviousTimeInMilliseconds(struct Clock *clock, uint32_t current_time_ms);

/**
 * Get the current time for the given clock, in milliseconds
 * @param clock The clock to get current time for
 * @return The current time for the given clock, in milliseconds
 */
uint32_t App_SharedClock_GetCurrentTimeInMilliseconds(const struct Clock *clock);

/**
 * Get the current time for the given clock, in seconds
 * @note When the time overflows, the number of seconds won't be exact because
 *       2^32 - 1 doesn't divide by 1000 exactly. In practice, this shouldn't be
 *       an issue for our use-case because it takes 50 days to overflow.
 * @param clock The clock to get current time for
 * @return The current time for the given clock, in seconds
 */
uint32_t App_SharedClock_GetCurrentTimeInSeconds(const struct Clock *clock);

/**
 * Get the previous time for the given clock, in milliseconds
 * @param clock The clock to get the previous time for
 * @return The current time for the given clock, in milliseconds
 */
uint32_t App_SharedClock_GetPreviousTimeInMilliseconds(const struct Clock *clock);

/**
 * Get the previous time for the given clock, in seconds
 * @note When the time overflows, the number of seconds won't be exact because
 *       2^32 - 1 doesn't divide by 1000 exactly. In practice, this shouldn't be
 *       an issue for our use-case because it takes 50 days to overflow.
 * @param clock The clock to get the previous time for
 * @return The previous time for the given clock, in seconds
 */
uint32_t App_SharedClock_GetPreviousTimeInSeconds(const struct Clock *clock);
