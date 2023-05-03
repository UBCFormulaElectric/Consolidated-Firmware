#pragma once

#include <stdint.h>
#include "Io_Hal.h"
#include "math.h"
#include <string.h>
#include "App_EepromExitCode.h"

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
