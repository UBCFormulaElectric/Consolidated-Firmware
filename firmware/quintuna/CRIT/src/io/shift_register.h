#pragma once

#include "hw/gpio.h"

#define LED_DATA_LENGTH 4u
#define SEVEN_SEG_DATA_LENGTH 9u

void io_shift_register_led_init(void);
void io_shift_register_seven_seg_init(void);
void io_shift_register_updateLedRegisters(uint8_t *data);
void io_shift_register_updateSevenSegRegisters(uint8_t *data);
void io_shift_register_latchOutput(const Gpio *rck);
void io_shift_register_led_setDimming(float brightness_percent);
