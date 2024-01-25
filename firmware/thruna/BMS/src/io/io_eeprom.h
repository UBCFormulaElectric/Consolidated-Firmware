#pragma once

#include <stdint.h>

#define EEPROM_PAGE_SIZE ((uint16_t)16U) // in Bytes

typedef enum
{
    EEPROM_STATUS_OK,
    EEPROM_STATUS_ADDR_ERROR,
    EEPROM_STATUS_I2C_ERROR,
    EEPROM_STATUS_SIZE_ERROR,
    EEPROM_STATUS_DATA_CORRUPT,
} EepromStatus;

/**
 * Write to EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to write in bytes
 * @param size the size of the data to be written in bytes
 * @return EepromStatus returns success status for debug
 */
EepromStatus io_eeprom_writePage(uint16_t page, uint8_t offset, uint8_t* data, uint16_t size);

/**
 * Read from EEPROM
 * @param page the number of the start page. Range from 0 to NUM_PAGES-1
 * @param offset the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @param data the pointer to the data to read in bytes
 * @param size the size of the data to be read in bytes
 * @return EepromStatus returns success status for debug
 */
EepromStatus io_eeprom_readPage(uint16_t page, uint8_t offset, uint8_t* data, uint16_t size);

/**
 * Erase page in EEPROM
 * @param page the number of the start page to set to all zeroes
 * @return EepromStatus returns success status for debug
 */
EepromStatus io_eeprom_pageErase(uint16_t page);
