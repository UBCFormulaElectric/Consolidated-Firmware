#pragma once

#include <stdint.h>
#include "Io_Hal.h"
#include "math.h"
#include <string.h>

/**
 * Write to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to write in bytes
 * @param size the size of the data to be written in bytes
 */
void EEPROM_Write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);

/**
 * Read from EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to read in bytes
 * @param size the size of the data to be read in bytes
 */
void Io_Eeprom_Read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);

/**
 * Erase page in EEPROM
 * @param page the number of the start page to set to all zeroes
 */
void Io_Eeprom_PageErase(uint16_t page);

/**
 * Write float value to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the float value to write to EEPROM
 */
void Io_Eeprom_WriteFloat(uint16_t page, uint16_t offset, float data);

/**
 * Read float value to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the float value to read from EEPROM
 */
float Io_Eeprom_ReadFloat(uint16_t page, uint16_t offset);