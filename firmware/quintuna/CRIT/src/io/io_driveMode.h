#pragma once
#include <stdint.h>

/**
 * @brief Initialize the encoder.
 */
void rotaryEncoder_init();

/**
 * @brief Update the encoder state.
 * @param channelA updated channel A value
 * @param channelB updated channe; B value
 */
void rotaryEncoder_update(uint8_t channelA, uint8_t channelB);

/**
 * @brief Get the current position.
 */
int32_t rotaryEncoder_getPosition();
