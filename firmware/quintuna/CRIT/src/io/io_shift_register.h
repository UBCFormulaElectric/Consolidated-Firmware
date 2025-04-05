#pragma once

#include "hw_gpio.h"

void io_shift_register_led_init(void);
void io_shift_register_seven_seg_init(void);
void io_shift_register_updateLedRegisters(uint8_t *data, uint8_t numBytes);
void io_shift_register_updateSevenSegRegisters(uint8_t *data, uint8_t numBytes);
void io_shift_register_latchOutput(const Gpio *rck);
void io_shift_register_seven_seg_setDimming(float brightness_percent);
void io_shift_register_led_setDimming(float brightness_percent);
