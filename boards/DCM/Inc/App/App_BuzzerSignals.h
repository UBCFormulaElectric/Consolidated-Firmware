#pragma once

#include <stdbool.h>

struct DcmWorld;

/**
 * Check if the buzzer in the given world is turned on
 * @param world The world containing the buzzer to check for
 * @return true if the buzzer is on, false if it is not
 */
bool App_BuzzerSignals_IsOn(struct DcmWorld *world);

/**
 * The callback function for the wait signal associated with the buzzer
 * @param world The world containing the buzzer
 */
void App_BuzzerSignals_Callback(struct DcmWorld *world);
