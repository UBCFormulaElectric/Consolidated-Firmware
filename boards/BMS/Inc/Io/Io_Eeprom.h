#pragma once

#include <stdint.h>
#include "Io_Hal.h"
#include "math.h"
#include <string.h>

#define PAGE_SIZE ((uint16_t)16U) // in Bytes

/**
 * @brief  EEPROM Status structures definition
 */
typedef enum
{
    EEPROM_OK         = 0x00U,
    EEPROM_ADDR_ERROR = 0x01U,
    EEPROM_I2C_ERROR  = 0x02U,
    EEPROM_SIZE_ERROR = 0x03U
} EEPROM_StatusTypeDef;

/**
 * Write to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to write in bytes
 * @param size the size of the data to be written in bytes
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef Io_Eeprom_WritePage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size);

/**
 * Read from EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to read in bytes
 * @param size the size of the data to be read in bytes
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef Io_Eeprom_ReadPage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size);

/**
 * Erase page in EEPROM
 * @param page the number of the start page to set to all zeroes
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef Io_Eeprom_PageErase(uint16_t page);

/**
 * Write float values to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, should ideally be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to write to EEPROM
 * @param num_floats number of floats to write
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef Io_Eeprom_WriteFloats(uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats);

/**
 * Read float values to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1, should ideally be 0 or multiple of 4 to
 * align with 4-byte size of float values
 * @param input_data pointer to array of floats to read from EEPROM
 * @param num_floats number of floats to read
 * @return EEPROM_StatusTypeDef returns success status for debug
 */
EEPROM_StatusTypeDef Io_Eeprom_ReadFloats(uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats);