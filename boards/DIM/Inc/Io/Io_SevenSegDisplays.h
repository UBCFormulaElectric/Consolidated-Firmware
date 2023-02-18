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
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_L_LeftHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle left-most 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_M_LeftHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right left-most 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_R_LeftHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_L_MiddleHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middle 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_M_MiddleHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * rightmost of the middle-trio 7-segment display during the next
 * Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_R_MiddleHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * right 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_L_RightHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * middlemost of the right-trio 7-segment display during the next Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_M_RightHexDigit(struct SevenSegHexDigit hex_digit);

/**
 * Using the given hexadecimal digit, update the command value to send to the
 * rightmost of the right-trio 7-segment display during the next
 * Io_SevenSegDisplays_WriteCommands()
 * @param hex_digit The hexadecimal digit to display
 */
void Io_SevenSegDisplays_Set_R_RightHexDigit(struct SevenSegHexDigit hex_digit);
