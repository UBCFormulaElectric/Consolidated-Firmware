#include "App_Eeprom.h"

#define SOC_ADDR_PAGE (0U)
#define SOC_ADDR_OFFSET (0U)
#define SOC_ADDR_SIZE_BYTES (1U)
#define NUM_PAGES (128U)
#define SAVED_COPIES 3U
#define BYTES_PER_FLOAT sizeof(float) / sizeof(uint8_t)
#define BYTES_PER_SHORT sizeof(uint16_t) / sizeof(uint8_t)
#define SHORT_SIZE ((uint8_t)(sizeof(uint16_t)))

static void convert_float_to_bytes(uint8_t *bytes, float float_to_convert)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        float   float_val;
        uint8_t bytes[BYTES_PER_FLOAT];
    } u;

    // place float input into union
    u.float_val = float_to_convert;

    for (uint8_t i = 0; i < BYTES_PER_FLOAT; i++)
    {
        // convert to array of bytes by accessing float value in union in byte-size increments (pun-intended)
        bytes[i] = u.bytes[i];
    }
}

// IF THIS FUNCTION IS MODIFIED, UPDATE VERSION LOCATED IN Test_Eeprom.cpp, THEY ARE MEANT TO BE THE SAME
static float convert_bytes_to_float(uint8_t *bytes_to_convert)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        float   float_val;
        uint8_t bytes[BYTES_PER_FLOAT];
    } u;

    for (uint8_t i = 0; i < BYTES_PER_FLOAT; i++)
    {
        u.bytes[i] = bytes_to_convert[i];
    }

    return u.float_val;
}

static void convert_short_to_bytes(uint8_t *bytes, uint16_t short_to_convert)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        uint16_t short_val;
        uint8_t  bytes[BYTES_PER_SHORT];
    } u;

    // place float input into union
    u.short_val = short_to_convert;

    for (uint8_t i = 0; i < BYTES_PER_SHORT; i++)
    {
        // convert to array of bytes by accessing float value in union in byte-size increments (pun-intended)
        bytes[i] = u.bytes[i];
    }
}

static uint16_t convert_bytes_to_short(uint8_t *bytes_to_convert)
{
    // Create union that stores float and byte array in same memory location.
    // This allows you to access 8-bit segments of the float value using array indexing
    union
    {
        uint16_t short_val;
        uint8_t  bytes[BYTES_PER_SHORT];
    } u;

    for (uint8_t i = 0; i < BYTES_PER_SHORT; i++)
    {
        u.bytes[i] = bytes_to_convert[i];
    }

    return u.short_val;
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
    // for floats, offset should be aligned to multiple of 4
    else if (offset % 4 != 0)
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

    for (uint8_t i = 0; i < num_bytes; i += SHORT_SIZE)
    {
        convert_short_to_bytes(&byte_array[i], address);
    }

    return eeprom->write_page(page, offset, byte_array, num_bytes);
}

EEPROM_StatusTypeDef App_Eeprom_ReadAddress(struct Eeprom *eeprom, uint16_t page, uint16_t *address)
{
    uint16_t num_bytes = SAVED_COPIES * 2; // saving 3 copies of address, each 2 bytes
    uint8_t  byte_array[num_bytes];
    uint8_t  offset = 0;
    uint8_t  read_status;

    read_status = eeprom->read_page(page, offset, byte_array, num_bytes);

    uint16_t short_array[SAVED_COPIES];

    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        short_array[i] = convert_bytes_to_short(&byte_array[i * 2]);
    }

    // TODO: Implement error checking using 3 copies of address stored in short_array
    *address = short_array[0];

    return read_status;
}
