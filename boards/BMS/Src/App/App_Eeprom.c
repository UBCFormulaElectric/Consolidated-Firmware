#include "App_Eeprom.h"
#include "string.h"

#define SOC_ADDR_PAGE (0U)
#define SOC_ADDR_OFFSET (0U)
#define SOC_ADDR_SIZE_BYTES (1U)
#define NUM_PAGES (128U)
#define SAVED_COPIES 3U
#define BYTES_PER_FLOAT sizeof(float) / sizeof(uint8_t)
#define BYTES_PER_SHORT sizeof(uint16_t) / sizeof(uint8_t)
#define SHORT_SIZE ((uint8_t)sizeof(uint16_t))

// expose static functions for testing
#ifdef TESTING
#define STATIC
#else
#define STATIC static
#endif

STATIC void convert_float_to_bytes(uint8_t *byte_array, float float_to_convert)
{
    memcpy(byte_array, (uint8_t *)(&float_to_convert), sizeof(float));
}

STATIC float convert_bytes_to_float(uint8_t *byte_array)
{
    float converted_float;
    memcpy(&converted_float, byte_array, sizeof(converted_float));
    return converted_float;
}

STATIC void convert_short_to_bytes(uint8_t *byte_array, uint16_t short_to_convert)
{
    memcpy(byte_array, (uint8_t *)(&short_to_convert), sizeof(uint16_t));
}

STATIC uint16_t convert_bytes_to_short(uint8_t *bytes_to_convert)
{
    uint16_t converted_short;
    memcpy(&converted_short, bytes_to_convert, sizeof(converted_short));
    return converted_short;
}

struct Eeprom
{
    EEPROM_StatusTypeDef (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t);
    EEPROM_StatusTypeDef (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t);
    EEPROM_StatusTypeDef (*page_erase)(uint16_t);
};

struct Eeprom *App_Eeprom_Create(
    EEPROM_StatusTypeDef (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    EEPROM_StatusTypeDef (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    EEPROM_StatusTypeDef (*page_erase)(uint16_t))
{
    struct Eeprom *eeprom = malloc(sizeof(struct Eeprom));
    assert(eeprom != NULL);

    eeprom->write_page = write_page;
    eeprom->read_page  = read_page;
    eeprom->page_erase = page_erase;

    return eeprom;
}

void App_Eeprom_Destroy(struct Eeprom *eeprom)
{
    free(eeprom);
}

EEPROM_StatusTypeDef
    App_Eeprom_WriteFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats)
{
    // Check if data to write fits into single page
    if (num_floats * sizeof(float) > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }
    // for floats, offset should be aligned to multiple of float size
    else if (offset % sizeof(float) != 0)
    {
        return EEPROM_ADDR_ERROR;
    }

    // convert float data to bytes array
    uint8_t data_bytes[num_floats * sizeof(float)];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        convert_float_to_bytes(&data_bytes[i * sizeof(float)], input_data[i]);
    }

    // write bytes array to EEPROM
    return eeprom->write_page(page, offset, data_bytes, (uint16_t)(num_floats * sizeof(float)));
}

EEPROM_StatusTypeDef
    App_Eeprom_ReadFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats)
{
    // Check if data to read fits into single page
    if (num_floats * sizeof(float) + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    uint8_t data[num_floats * sizeof(float)];
    for (int i = 0; i < num_floats; i++)
    {
        data[i] = 0;
    }
    uint8_t read_status;

    read_status = eeprom->read_page(page, offset, data, (uint16_t)(num_floats * sizeof(float)));

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_data[i] = convert_bytes_to_float(&data[i * sizeof(float)]);
    }

    return read_status;
}

EEPROM_StatusTypeDef App_Eeprom_PageErase(struct Eeprom *eeprom, uint16_t page)
{
    return eeprom->page_erase(page);
}

EEPROM_StatusTypeDef App_Eeprom_WriteAddress(struct Eeprom *eeprom, uint16_t page, uint16_t address)
{
    uint16_t num_bytes = SAVED_COPIES * 2; // saving 3 copies of address, each 2 bytes
    uint8_t  byte_array[num_bytes];
    uint8_t  offset = 0;

    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        convert_short_to_bytes(&byte_array[i * 2], address);
    }

    return eeprom->write_page(page, offset, byte_array, num_bytes);
}

EEPROM_StatusTypeDef App_Eeprom_ReadAddresses(struct Eeprom *eeprom, uint16_t page, uint16_t *addresses)
{
    uint16_t num_bytes = SAVED_COPIES * sizeof(uint16_t); // saving 3 copies of address, each 2 bytes
    uint8_t  byte_array[num_bytes];
    uint8_t  offset = 0;
    uint8_t  read_status;

    read_status = eeprom->read_page(page, offset, byte_array, num_bytes);

    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        addresses[i] = convert_bytes_to_short(&byte_array[i * 2]);
    }

    return read_status;
}
