#include "Io_Eeprom.h"
#include "App_SharedMacros.h"

// Define the I2C
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

#define NUM_BYTES (2048U)                                         // total number of bytes on EEPROM
#define PAGE_ADDR_START_BIT ((uint16_t)(log(PAGE_SIZE) / log(2))) // number of bit where addressing starts
#define MEM_ACCESS_TIMEOUT (1000U)

static void convert_float_to_bytes(uint8_t *bytes, float float_to_convert)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        float   float_val;
        uint8_t bytes[4];
    } u;

    // place float input into union
    u.float_val = float_to_convert;

    for (int i = 0; i < 4; i++)
    {
        // convert to array of bytes by accessing float value in union in byte-size increments (pun-intended)
        bytes[i] = u.bytes[i];
    }
}

static float convert_bytes_to_float(uint8_t *bytes)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        float   float_val;
        uint8_t bytes[4];
    } u;

    for (int i = 0; i < 4; i++)
    {
        u.bytes[i] = bytes[i];
    }

    return u.float_val;
}

EEPROM_StatusTypeDef Io_Eeprom_WritePage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef i2c_status;

    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT | offset);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_ADDR_ERROR;
    }
    // if size of data exceeds page, return error
    if (size + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    i2c_status =
        HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, data, size, MEM_ACCESS_TIMEOUT);

    // If mem write error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_I2C_ERROR;
    }

    return EEPROM_OK;
}

EEPROM_StatusTypeDef Io_Eeprom_ReadPage(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef i2c_status;

    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT | offset);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_ADDR_ERROR;
    }
    // if size of data exceeds page, return error
    if (size + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    i2c_status =
        HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, data, size, MEM_ACCESS_TIMEOUT);

    // If mem read error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_I2C_ERROR;
    }

    return EEPROM_OK;
}

EEPROM_StatusTypeDef Io_Eeprom_PageErase(uint16_t page)
{
    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT);

    // if address beyond EEPROM address space, return error
    if (mem_address >= NUM_BYTES)
    {
        return EEPROM_ADDR_ERROR;
    }

    // create empty data to write to EEPROM
    uint8_t data[PAGE_SIZE];
    memset(data, 0xff, PAGE_SIZE);

    HAL_StatusTypeDef i2c_status;

    // write the data to the EEPROM
    i2c_status = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, mem_address, 2, data, PAGE_SIZE, 1000);

    // If mem write error detected, return error
    if (i2c_status != HAL_OK)
    {
        return EEPROM_ADDR_ERROR;
    }

    // Write cycle delay
    HAL_Delay(5);

    return EEPROM_OK;
}

EEPROM_StatusTypeDef Io_Eeprom_WriteFloats(uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats)
{
    // Check if data to write fits into single page
    if (num_floats * sizeof(float) + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    // convert float to bytes array
    uint8_t data_bytes[num_floats * sizeof(float)];
    for (uint8_t i = 0; i < num_floats; i++)
    {
        convert_float_to_bytes(&data_bytes[i * sizeof(float)], input_data[i]);
    }

    // write bytes array to EEPROM
    return Io_Eeprom_WritePage(page, offset, data_bytes, sizeof(float));
}

EEPROM_StatusTypeDef Io_Eeprom_ReadFloats(uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats)
{
    // Check if data to read fits into single page
    if (num_floats * sizeof(float) + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    uint8_t data[num_floats * sizeof(float)];

    EEPROM_StatusTypeDef read_status;

    read_status = Io_Eeprom_ReadPage(page, offset, data, num_floats * sizeof(float));

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_data[i] = convert_bytes_to_float(&data[i * sizeof(float)]);
    }

    return read_status;
}
