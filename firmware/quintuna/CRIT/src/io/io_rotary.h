#pragma once

#include <stdint.h>

/**
 * @brief Initialize the encoder.
 */
void io_rotary_init(void);

/**
 * @brief Update the encoder state.
 * @param channelA updated channel A value
 * @param channelB updated channe; B value
 */
int32_t io_rotary_toggleEventHandler(void);

/**
 * @brief Handle the even where the rotary is pressed.
 */
uint8_t io_rotary_pressEventHandler(void);
