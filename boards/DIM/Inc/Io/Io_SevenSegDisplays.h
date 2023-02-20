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
 * left left-most 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display i am doing a little test here
 *
 */
void Io_SevenSegDisplays_SetLeft_L_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * left 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetLeft_M_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * left 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetLeft_R_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetMiddle_L_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetMiddle_M_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetMiddle_R_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetRight_L_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetRight_M_HexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_SetRight_R_HexDigit(struct SevenSegHexDigit hex_digit);
