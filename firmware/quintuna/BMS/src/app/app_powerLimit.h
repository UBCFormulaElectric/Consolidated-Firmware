#pragma once

/**
 * @brief runs the power and current limit checking algorithm and updates all the current limit CAN messages
 */
void app_powerLimit_broadcast();