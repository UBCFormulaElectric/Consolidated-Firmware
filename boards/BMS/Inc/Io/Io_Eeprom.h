#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "Io_Hal.h"

void Io_Eeprom_Init(I2C_HandleTypeDef *i2c_handle);

bool Io_Eeprom_WriteRaw(uint16_t address_bytes, void *data, uint8_t size_bytes);

bool Io_Eeprom_ReadRaw(uint16_t address_bytes, void *data, uint8_t size_bytes);

bool Io_Eeprom_WriteFloat(uint16_t address, float value);

bool Io_Eeprom_ReadFloat(uint16_t address, float *value);
