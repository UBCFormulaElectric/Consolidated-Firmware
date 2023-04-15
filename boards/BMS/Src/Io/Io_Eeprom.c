#include "Io_Eeprom.h"

// Define the I2C
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// Define the Page Size and number of pages
#define PAGE_SIZE ((uint16_t)16U)                                 // in Bytes
#define NUM_PAGES (128U)                                          // number of pages
#define NUM_BYTES (2048U)                                         // total number of bytes on EEPROM
#define PAGE_ADDR_START_BIT ((uint16_t)(log(PAGE_SIZE) / log(2))) // number of bit where addressing starts
#define MEM_ACCESS_TIMEOUT (1000U)

// Function to determine the number of bytes to write/read to each page.
static uint16_t Bytes_To_Process(uint16_t size, uint16_t offset)
{
    if ((size + offset) < PAGE_SIZE)
    {
        return size;
    }
    else
    {
        return (uint16_t)(PAGE_SIZE - offset);
    }
}

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

uint8_t Io_Eeprom_WriteBytes(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    // determine the start page and ending page
    uint16_t startPage = page;
    uint16_t endPage   = (uint16_t)(page + ((size + offset - 1) / PAGE_SIZE));

    // determine the number of pages to be written
    uint16_t num_pages = (uint16_t)(endPage - startPage + 1);
    uint16_t pos       = 0U;

    HAL_StatusTypeDef i2c_status;

    for (int i = 0; i < num_pages; i++)
    {
        // determine the memory address location
        uint16_t mem_address = (uint16_t)(startPage << PAGE_ADDR_START_BIT | offset);

        // if address beyond EEPROM address space, return
        if (mem_address >= NUM_BYTES)
        {
            return 1;
        }
        // Determine the number of bytes to write to page
        uint16_t bytes_to_write = Bytes_To_Process(size, offset);

        i2c_status = HAL_I2C_Mem_Write(
            EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, &data[pos], bytes_to_write, MEM_ACCESS_TIMEOUT);

        // If mem write error detected, return 2
        if (i2c_status != HAL_OK)
        {
            return 2;
        }

        startPage++;                                // Increment the page for next write
        offset = 0U;                                // writing to new page, offset will be zero for next write.
        size   = (uint16_t)(size - bytes_to_write); // reduce size by the amount of bytes written to page
        pos    = (uint16_t)(pos + bytes_to_write);  // increase starting position by number of bytes written

        HAL_Delay(5); // Write cycle delay (5ms)
    }
    return 0;
}

uint8_t Io_Eeprom_ReadBytes(uint16_t page, uint8_t offset, uint8_t *data, uint16_t size)
{
    // determine the start page and ending page
    uint16_t startPage = page;
    uint16_t endPage   = (uint16_t)(page + ((size + offset - 1) / PAGE_SIZE));

    // determine the number of pages to be written
    uint16_t num_pages = (uint16_t)(endPage - startPage + 1);
    uint16_t pos       = 0U;

    HAL_StatusTypeDef i2c_status;

    for (uint16_t i = 0; i < num_pages; i++)
    {
        // determine the memory address location
        uint16_t mem_address = (uint16_t)(startPage << PAGE_ADDR_START_BIT | offset);

        // if address beyond EEPROM address space, return 1
        if (mem_address >= NUM_BYTES)
        {
            return 1;
        }

        // Determine the number of bytes to read from page
        uint16_t bytes_to_read = Bytes_To_Process(size, offset);

        i2c_status = HAL_I2C_Mem_Read(
            EEPROM_I2C, EEPROM_ADDR, mem_address, I2C_MEMADD_SIZE_8BIT, &data[pos], bytes_to_read, MEM_ACCESS_TIMEOUT);

        // If mem read error detected, return 2
        if (i2c_status != HAL_OK)
        {
            return 2;
        }

        startPage++;                               // Increment the page for next read
        offset = 0U;                               // reading from new page, offset will be zero for next read.
        size   = (uint16_t)(size - bytes_to_read); // reduce size by the amount of bytes read
        pos    = (uint16_t)(pos + bytes_to_read);  // increase starting position by number of bytes read
    }

    return 0;
}

uint8_t Io_Eeprom_PageErase(uint16_t page)
{
    // determine the memory address location
    uint16_t mem_address = (uint16_t)(page << PAGE_ADDR_START_BIT);

    if (mem_address >= NUM_BYTES)
    {
        return 1;
    }

    // create empty data to write to EEPROM
    uint8_t data[PAGE_SIZE];
    memset(data, 0xff, PAGE_SIZE);

    HAL_StatusTypeDef i2c_status;

    // write the data to the EEPROM
    i2c_status = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, mem_address, 2, data, PAGE_SIZE, 1000);

    // If mem write error detected, return 2
    if (i2c_status != HAL_OK)
    {
        return 2;
    }

    // Write cycle delay
    HAL_Delay(5);

    return 0;
}

void Io_Eeprom_WriteFloat(uint16_t page, uint8_t offset, float data_float)
{
    // convert float to bytes array
    uint8_t data_bytes[sizeof(float)];
    convert_float_to_bytes(data_bytes, data_float);

    // write bytes array to EEPROM
    Io_Eeprom_WriteBytes(page, offset, data_bytes, sizeof(float));
}

float Io_Eeprom_ReadFloat(uint16_t page, uint8_t offset)
{
    uint8_t data[4];

    Io_Eeprom_ReadBytes(page, offset, data, sizeof(float));
    return convert_bytes_to_float(data);
}
