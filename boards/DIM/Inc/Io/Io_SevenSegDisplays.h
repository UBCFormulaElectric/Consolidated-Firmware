#pragma once

#include <stdbool.h>
#include "App_SevenSegDisplay.h"

void Io_SevenSegDisplays_Init(SPI_HandleTypeDef *hspi);
void Io_SevenSegDisplays_WriteCommands(void);
void Io_SevenSegDisplays_SetLeftHexDigit(struct SevenSegHexDigit hex_digit);
void Io_SevenSegDisplays_SetMiddleHexDigit(struct SevenSegHexDigit hex_digit);
void Io_SevenSegDisplays_SetRightHexDigit(struct SevenSegHexDigit hex_digit);
