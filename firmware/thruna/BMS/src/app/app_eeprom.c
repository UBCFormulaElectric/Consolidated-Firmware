#include "app_eeprom.h"
#include "io_eeprom.h"
#include <string.h>

#define NUM_PAGES (16U) // TODO: Investigate why addresses above 16 run into corruption issues (should be 128 pages)
#define SAVED_COPIES 4U
#define BYTES_PER_FLOAT sizeof(float) / sizeof(uint8_t)
#define BYTES_PER_SHORT sizeof(uint16_t) / sizeof(uint8_t)
#define DEFAULT_OFFSET 0U
#define ADDRESS_PAGE 0U
#define MAX_SOC_ADDR (NUM_PAGES - 1U)

static void convert_float_to_bytes(uint8_t* byte_array, float float_to_convert)
{
    memcpy(byte_array, (uint8_t*)(&float_to_convert), sizeof(float));
}

static float convert_bytes_to_float(uint8_t* byte_array)
{
    float converted_float;
    memcpy(&converted_float, byte_array, sizeof(float));
    return converted_float;
}

static void convert_short_to_bytes(uint8_t* byte_array, uint16_t short_to_convert)
{
    memcpy(byte_array, (uint8_t*)(&short_to_convert), sizeof(uint16_t));
}

static uint16_t convert_bytes_to_short(uint8_t* bytes_to_convert)
{
    uint16_t converted_short;
    memcpy(&converted_short, bytes_to_convert, sizeof(uint16_t));
    return converted_short;
}

EepromStatus app_eeprom_writeFloats(uint16_t page, uint8_t offset, float* input_data, uint8_t num_floats)
{
    // Check if data to write fits into single page
    if (num_floats * sizeof(float) > EEPROM_PAGE_SIZE)
    {
        return EEPROM_STATUS_SIZE_ERROR;
    }
    // for floats, offset should be aligned to multiple of float size
    else if (offset % sizeof(float) != 0)
    {
        return EEPROM_STATUS_ADDR_ERROR;
    }

    // convert float data to bytes array
    uint8_t data_bytes[num_floats * sizeof(float)];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        convert_float_to_bytes(&data_bytes[i * sizeof(float)], input_data[i]);
    }

    // write bytes array to EEPROM
    return io_eeprom_writePage(page, offset, data_bytes, (uint16_t)(num_floats * sizeof(float)));
}

EepromStatus app_eeprom_readFloats(uint16_t page, uint8_t offset, float* output_data, uint8_t num_floats)
{
    // Check if data to read fits into single page
    if (num_floats * sizeof(float) + offset > EEPROM_PAGE_SIZE)
    {
        return EEPROM_STATUS_SIZE_ERROR;
    }

    uint8_t data[num_floats * sizeof(float)];
    for (int i = 0; i < num_floats; i++)
    {
        data[i] = 0;
    }
    EepromStatus read_status;

    read_status = io_eeprom_readPage(page, offset, data, (uint16_t)(num_floats * sizeof(float)));

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_data[i] = convert_bytes_to_float(&data[i * sizeof(float)]);
    }

    return read_status;
}

EepromStatus app_eeprom_pageErase(uint16_t page)
{
    return io_eeprom_pageErase(page);
}

EepromStatus app_eeprom_updateSavedSocAddress(uint16_t* address)
{
    uint16_t num_bytes = SAVED_COPIES * sizeof(uint16_t); // saving 3 copies of address, each 2 bytes
    uint8_t  byte_array[num_bytes];

    // Increment address for wear levelling purposes
    if (*address >= MAX_SOC_ADDR)
    {
        *address = DEFAULT_SOC_ADDR;
    }
    else
    {
        (*address)++;
    }

    // convert address to bytes and write 4 copies to page 0

    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        convert_short_to_bytes(&byte_array[i * sizeof(uint16_t)], *address);
    }

    return io_eeprom_writePage(ADDRESS_PAGE, DEFAULT_OFFSET, byte_array, num_bytes);
}

ExitCode app_eeprom_readSocAddress(uint16_t* address)
{
    ExitCode retval = EXIT_CODE_OK;

    if (*address >= NUM_PAGES)
    {
        return EXIT_CODE_ERROR;
    }

    uint16_t     num_bytes = SAVED_COPIES * sizeof(uint16_t); // saving 3 copies of address, each 2 bytes
    uint8_t      byte_array[num_bytes];
    uint16_t     address_copies[SAVED_COPIES];
    EepromStatus read_status;

    // Read saved address
    read_status = io_eeprom_readPage(ADDRESS_PAGE, DEFAULT_OFFSET, byte_array, num_bytes);

    // Convert read bytes to shorts
    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        address_copies[i] = convert_bytes_to_short(&byte_array[i * sizeof(uint16_t)]);
    }

    // Determine if read was successful and if address is uncorrupted
    if (read_status != EEPROM_STATUS_OK)
    {
        *address = 1;
        retval   = EXIT_CODE_ERROR;
    }
    else if (
        address_copies[0] == address_copies[1] || address_copies[0] == address_copies[2] ||
        address_copies[0] == address_copies[3])
    {
        *address = address_copies[0];
        retval   = EXIT_CODE_OK;
    }
    else if (address_copies[1] == address_copies[2] || address_copies[1] == address_copies[3])
    {
        *address = address_copies[1];
        retval   = EXIT_CODE_OK;
    }
    else if (address_copies[2] == address_copies[3])
    {
        *address = address_copies[2];
        retval   = EXIT_CODE_OK;
    }
    else
    {
        *address = 1;
        retval   = EXIT_CODE_ERROR;
    }

    return retval;
}

EepromStatus app_eeprom_writeMinSoc(float min_soc, uint16_t address)
{
    float float_arr[SAVED_COPIES];

    for (uint8_t i = 0U; i < SAVED_COPIES; i++)
    {
        float_arr[i] = min_soc;
    }

    return app_eeprom_writeFloats(address, DEFAULT_OFFSET, float_arr, SAVED_COPIES);
}

ExitCode app_eeprom_readMinSoc(uint16_t address, float* min_soc)
{
    float        soc_copies[SAVED_COPIES];
    EepromStatus read_status = app_eeprom_readFloats(address, DEFAULT_OFFSET, soc_copies, SAVED_COPIES);

    if (read_status != EEPROM_STATUS_OK)
    {
        *min_soc = -1;
    }
    else if (soc_copies[0] == soc_copies[1] || soc_copies[0] == soc_copies[2] || soc_copies[0] == soc_copies[3])
    {
        *min_soc = soc_copies[0];
    }
    else if (soc_copies[1] == soc_copies[2] || soc_copies[1] == soc_copies[3])
    {
        *min_soc = soc_copies[1];
    }
    else if (soc_copies[2] == soc_copies[3])
    {
        *min_soc = soc_copies[2];
    }
    else
    {
        *min_soc = -1;
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
}
