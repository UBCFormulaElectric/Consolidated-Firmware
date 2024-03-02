#include "io_eeprom.h"
#include "app_utils.h"
#include "main.h"
#include "hw_hal.h"

#include <math.h>
#include <string.h>

// Define the I2C
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

#define NUM_BYTES (2048U)                                                // total number of bytes on EEPROM
#define PAGE_ADDR_START_BIT ((uint16_t)(log(EEPROM_PAGE_SIZE) / log(2))) // number of bit where addressing starts
#define MEM_ACCESS_TIMEOUT (1000U)

EepromStatus io_eeprom_writePage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT | offset);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_STATUS_ADDR_ERROR;
    }
    // if size of data exceeds page, return error
    if (size + offset > EEPROM_PAGE_SIZE)
    {
        return EEPROM_STATUS_SIZE_ERROR;
    }

    const HAL_StatusTypeDef i2c_status =
        HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, data, size, MEM_ACCESS_TIMEOUT);

    // If mem write error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_STATUS_I2C_ERROR;
    }

    return EEPROM_STATUS_OK;
}

EepromStatus io_eeprom_readPage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT | offset);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_STATUS_ADDR_ERROR;
    }
    // if size of data exceeds page, return error
    if (size + offset > EEPROM_PAGE_SIZE)
    {
        return EEPROM_STATUS_SIZE_ERROR;
    }

    const HAL_StatusTypeDef i2c_status =
        HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, data, size, MEM_ACCESS_TIMEOUT);

    // If mem read error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_STATUS_I2C_ERROR;
    }

    return EEPROM_STATUS_OK;
}

EepromStatus io_eeprom_pageErase(uint16_t page)
{
    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_STATUS_ADDR_ERROR;
    }

    // create empty data to write to EEPROM
    uint8_t data[EEPROM_PAGE_SIZE];
    memset(data, 0xff, EEPROM_PAGE_SIZE);

    // write the data to the EEPROM
    const HAL_StatusTypeDef i2c_status =
        HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, mem_address, 2, data, EEPROM_PAGE_SIZE, 1000);

    // If mem write error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_STATUS_ADDR_ERROR;
    }

    return EEPROM_STATUS_OK;
}
