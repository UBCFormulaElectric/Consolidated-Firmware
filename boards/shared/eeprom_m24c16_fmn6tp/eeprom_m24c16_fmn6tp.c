#pragma GCC diagnostic ignored "-Wunused-variable"

//.h file to be included
#include "eeprom_m24c16_fmn6tp.h"

// User PD
#define SOC_NUM_OF_BYTES 4
#define WRITE_LOC 3

// I2C Handle for the I2C device the EEPROM is connected to
// May or may not need
extern I2C_HandleTypeDef hi2c1;

// The I2c slave device addresses to use when reading/writing to/from the EEPROM
static const uint16_t EEPROM_READ_ADDR  = 0xA1; // 0b101_00001
static const uint16_t EEPROM_WRITE_ADDR = 0xA0; // 0b101_00000

// Define test read and write addresses

#define TEST_ADDR 0x00

HAL_StatusTypeDef Io_Eeprom_M24C16_writeToEeprom(
    uint16_t write_address,
    uint8_t *data,
    uint16_t data_size)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_WRITE_ADDR, 3, 1000);

    if (status == HAL_OK)
    {
        status = HAL_I2C_Mem_Write(
            &hi2c1, EEPROM_WRITE_ADDR, write_address, I2C_MEMADD_SIZE_8BIT,
            data, data_size, 1000);
    }

    return status;
}

HAL_StatusTypeDef Io_Eeprom_M24C16_readFromEeprom(
    uint16_t read_start_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Read(
        &hi2c1, EEPROM_READ_ADDR, read_start_address, I2C_MEMADD_SIZE_8BIT,
        data, data_size, 1000);
}
