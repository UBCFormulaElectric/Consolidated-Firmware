#pragma once

void Io_SevenSegDisplays_Init(SPI_HandleTypeDef *hspi);
void Io_SevenSegDisplays_WriteCommands(void);
void Io_SevenSegDisplays_SetLeftHexDigit(uint8_t hex_digit);
void Io_SevenSegDisplays_SetMiddleHexDigit(uint8_t hex_digit);
void Io_SevenSegDisplays_SetRightHexDigit(uint8_t hex_digit);
