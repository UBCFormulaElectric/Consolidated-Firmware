#pragma GCC diagnostic ignored "-Wunused-variable"

//.h file to be included
#include "eeprom_m24c16_fmn6tp.h"
#include "string.h"

// I2C Handle for the I2C device the EEPROM is connected to
// May or may not need
static I2C_HandleTypeDef *eeprom_i2c_handle;

// The I2c slave device addresses to use when reading/writing to/from the EEPROM

static const uint16_t EEPROM_READ_ADDR  = 0xA1; // 0b101_00001
static const uint16_t EEPROM_WRITE_ADDR = 0xA0; // 0b101_00000

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
        eeprom_i2c_handle, EEPROM_WRITE_ADDR, write_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);
}

HAL_StatusTypeDef Io_Eeprom_M24C16_readFromEeprom(
    uint16_t read_start_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Read(
        eeprom_i2c_handle, EEPROM_READ_ADDR, read_start_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);
}

HAL_StatusTypeDef initI2CHandler(I2C_HandleTypeDef *i2c_handle)
{
    eeprom_i2c_handle = i2c_handle;
    HAL_StatusTypeDef status;
    return status = HAL_OK;
}

// TODO: Split floating point representation into 4 bytes and write to memory
// Determine how the floating point representation will be input to the system
// (assume float for now)

uint8_t *floatToIntByte(float stateOfCharge)
{
    // static unsigned char c[sizeof stateOfCharge];
    static uint8_t b[sizeof stateOfCharge];
    memcpy(b, &stateOfCharge, sizeof stateOfCharge);
    return b;
}

HAL_StatusTypeDef Io_Eeprom_M24C16_testWriteRead(uint8_t data)
{
    uint8_t           data_test[3] = { 0x01, 0x02, 0x03 };
    uint8_t           data_read;
    uint8_t *         stateOfCharge;
    HAL_StatusTypeDef status = HAL_OK;

    stateOfCharge = floatToIntByte(0.1);

    // Printing to "use" the variable
    printf("%i", *(stateOfCharge));

    //====== Write to the EEPROM, then read ======

    // Test data to write to EEPROM
    printf("%i", data);

    // Perform page write
    status = Io_Eeprom_M24C16_writeToEeprom(0x00, stateOfCharge, 3);

    // Perform byte write
    // status = Io_Eeprom_M24C16_writeToEeprom(0x00,data,1);

    if (status != HAL_OK)
    {
        return status;
    }

    // Read data and store in data_read
    status = Io_Eeprom_M24C16_readFromEeprom(0x00, &data_read, 1);
    status = Io_Eeprom_M24C16_readFromEeprom(0x01, &data_read, 1);
    status = Io_Eeprom_M24C16_readFromEeprom(0x02, &data_read, 1);

    return status;
}
