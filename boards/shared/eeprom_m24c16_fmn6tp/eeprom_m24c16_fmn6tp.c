//.h file to be included
#include "eeprom_m24c16_fmn6tp.h"

//I2C Handle for the I2C device the EEPROM is connected to
//May or may not need
static I2C_HandleTypeDef *eeprom_i2c_handle;

// The I2c slave device addresses to use when reading/writing to/from the EEPROM

static const uint16_t EEPROM_READ_ADDR = 0xA1;  //0b101_00001
static const uint16_t EEPROM_WRITE_ADDR = 0xA0; //0b101_00000


// Define test read and write addresses

#define TEST_ADDR 0x00

/**
 * Send the given data to the EEPROM
 * @param write_address The byte address to write data
 * @param data The data to send
 * @param data_size The size of the "data" array
 */

// See if the following procedure for the EEPROM. Copy from IMU Code for now
HAL_StatusTypeDef Io_Eeprom_M24C16_writeToEeprom(
        uint16_t write_address,
        uint8_t *data,
        uint16_t data_size)
{

    return HAL_I2C_Mem_Write(
        eeprom_i2c_handle, EEPROM_WRITE_ADDR, write_address, I2C_MEMADD_SIZE_8BIT, data,
        data_size, 1000);

}

HAL_StatusTypeDef Io_Eeprom_M24C16_readFromEeprom(uint16_t read_start_address, uint8_t* data, uint16_t data_size){

    return HAL_I2C_Mem_Read(eeprom_i2c_handle, EEPROM_READ_ADDR, read_start_address, I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);

}

HAL_StatusTypeDef Io_Eeprom_M24C16_configureEeprom(I2C_HandleTypeDef *i2c_handle)
{

    eeprom_i2c_handle = i2c_handle;

    uint8_t data;
    uint8_t read_data;

    HAL_StatusTypeDef status = HAL_OK;


    //====== Initialize the EEPROM ======

    // Wait until the device is ready to communicate
    status = HAL_I2C_IsDeviceReady(eeprom_i2c_handle, EEPROM_READ_ADDR, 3,1000);

    //Byte Write 0x01 to the EEPROM Chip
    if(status != HAL_OK)
    {
        return HAL_ERROR;
    }


    data = 0x01;
    status = Io_Eeprom_M24C16_writeToEeprom(TEST_ADDR, &data, 1);

    //TODO: Check status of the chip before read?
    //Read 0 from the EEPROM Chip
    status = Io_Eeprom_M24C16_readFromEeprom(TEST_ADDR, &read_data, 1);

    return status;

}
