#pragma once

#include <stdbool.h>

struct OkStatus;

/**
 * Allocate and initialize a latch status
 * @param is_enabled A function that returns whether the latch is enabled or not
 * @return The created latch status, whose ownership is given to the caller
 */
struct OkStatus *App_OkStatus_Create(bool (*is_enabled)(void));

/**
 * Deallocate the memory used by the given latch status
 * @param ok_status The latch status to deallocate
 */
void App_OkStatus_Destroy(struct OkStatus *ok_status);

/**
 * Check if the given latch status is enabled
 * @param ok_status The latch status to check
 * @return true if the latch status is enabled, else false
 */
bool App_OkStatus_IsEnabled(const struct OkStatus *ok_status);
