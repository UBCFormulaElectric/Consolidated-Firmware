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
<<<<<<< HEAD
 * left 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
=======
 * left left-most 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display i am doing a little test here
 *
>>>>>>> 9bf9406 (git test)
 */
void Io_SevenSegDisplays_SetLeftHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetMiddleHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetRightHexDigit(struct SevenSegHexDigit hex_digit);
