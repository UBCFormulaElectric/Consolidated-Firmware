#pragma once

#include "App_SevenSegDisplay.h"

/**
 * Register SPI bus used to communicate with the 7-segment display hardware
 * @param hspi The SPI bus to register
 */
void Io_SevenSegDisplays_Init(void);

/**
 * Issue commands to the shift registers controlling the 7-segment displays via
 * the registered SPI bus
 */
void Io_SevenSegDisplays_WriteCommands(void);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * specified 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 * @param DIGIT The enum of the 7-segment display to write too
 */

void Io_SevenSegDisplays_SetHexDigit(struct SevenSegHexDigit hex_digit, int DIGIT);

void Io_SevenSegDisplays_Populate_Commands(struct SevenSegDisplays *seven_displays, int DIGITS[]);