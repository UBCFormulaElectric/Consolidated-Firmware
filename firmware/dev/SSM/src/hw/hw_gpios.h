#pragma once
#include "hw_gpio.h"

/* D-OUT. */

// GPIO output pin 1.
extern const Gpio dout_1;

// GPIO output pin 2.
extern const Gpio dout_2;

// GPIO output pin 3.
extern const Gpio dout_3;

// GPIO output pin 4.
extern const Gpio dout_4;

/* LEDS. */

// Debug led.
extern const Gpio debug_led;

// Boot led.
extern const Gpio boot_led;

// Indicator 1.
extern const Gpio indicator_1;

// Indicator 2.
extern const Gpio indicator_2;

// Indicator 3.
extern const Gpio indicator_3;

/* Chip-Selects. */

// Isospi low side chip-select.
extern const Gpio isospi_chipSelect_ls;

// Isospi high side chip-select.
extern const Gpio isospi_chipSelect_hs;

// DAC chip-select.
// - When low, commands are accepted.
// - When high, command is executed.
extern const Gpio dac_chipSelect;

/* Interlocks (low breaks interlock). */

// Interlock 1.
extern const Gpio interlock_1;

// Interlock 2.
extern const Gpio interlock_2;

// Interlock 3.
extern const Gpio interlock_3;

// Interlock 4.
extern const Gpio interlock_4;

/* DAC Clear (Active low, hold high to maintain data). */
extern const Gpio dac_n_clear;
