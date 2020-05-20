#pragma once

#include <stdbool.h>

struct OkStatus;

/**
 * Allocate and initialize a latch status
 * @param is_enabled A function that returns whether the latch is enabled or not
 * @return The created latch status, whose ownership is given to the caller
 */
struct OkStatus *App_LatchStatus_Create(bool (*is_enabled)(void));

/**
 * Deallocate the memory used by the given latch status
 * @param latch_status The latch status to deallocate
 */
void App_LatchStatus_Destroy(struct OkStatus *latch_status);

/**
 * Check if the given latch status is enabled
 * @param latch_status The latch status to check
 * @return true if the latch status is enabled, else false
 */
bool App_LatchStatus_IsEnabled(const struct OkStatus *latch_status);
