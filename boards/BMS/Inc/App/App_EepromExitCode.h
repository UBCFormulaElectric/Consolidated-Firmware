#pragma once

/**
 * @brief  EEPROM Status structures definition
 */
typedef enum {
  EEPROM_OK = 0x00U,
  EEPROM_ADDR_ERROR = 0x01U,
  EEPROM_I2C_ERROR = 0x02U,
  EEPROM_SIZE_ERROR = 0x03U,
  EEPROM_DATA_CORRUPT = 0x04U
} EEPROM_StatusTypeDef;
