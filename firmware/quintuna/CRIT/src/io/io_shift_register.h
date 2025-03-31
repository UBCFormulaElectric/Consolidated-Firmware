#pragma once

#include "hw_gpio.h"

void io_shift_register_updateLedRegisters(uint8_t *data, uint8_t numBytes);
void io_shift_register_updateSevenSegRegisters(uint8_t *data, uint8_t numBytes);
void io_shift_register_latchOutput(const Gpio *rck);
void io_shift_register_setDimming(const Gpio *dimming, bool state);
