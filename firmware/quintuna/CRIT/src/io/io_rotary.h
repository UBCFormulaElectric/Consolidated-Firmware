#pragma once

#include <stdint.h>

/**
 * @brief Initialize the rotary.
 */
void io_rotary_init(void);

/**
 * @brief Update the rotary state.
 * @param channelA updated channel A value
 * @param channelB updated channe; B value
 */
uint32_t io_rotary_toggleEventHandler(void);

/**
 * @brief Handle the event where the rotary is pressed.
 */
uint8_t io_rotary_pressEventHandler(void);
