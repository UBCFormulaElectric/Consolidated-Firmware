#include "App_Eeprom.h"

#define SOC_ADDR_PAGE (0U)
#define SOC_ADDR_OFFSET (0U)
#define SOC_ADDR_SIZE_BYTES (1U)

#define NUM_PAGES (128U)

/**
 * @brief  EEPROM Status structures definition
 */
typedef enum
{
    EEPROM_OK         = 0x00U,
    EEPROM_ADDR_ERROR = 0x01U,
    EEPROM_I2C_ERROR  = 0x02U,
    EEPROM_SIZE_ERROR = 0x03U
} EEPROM_StatusTypeDef;

struct Eeprom
{
    uint8_t (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t);
    uint8_t (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t);
    uint8_t (*page_erase)(uint16_t);
};

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

struct Eeprom *App_Eeprom_Create(
    uint8_t (*write_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    uint8_t (*read_page)(uint16_t, uint8_t, uint8_t *, uint16_t),
    uint8_t (*page_erase)(uint16_t))
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

uint8_t
    App_Eeprom_WriteFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *input_data, uint8_t num_floats)
{
    // Check if data to write fits into single page
    if (num_floats * sizeof(float) + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    // convert float data to bytes array
    uint8_t data_bytes[num_floats * sizeof(float)];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        convert_float_to_bytes(&data_bytes[i * sizeof(float)], input_data[i]);
    }

    // write bytes array to EEPROM
    //    return Io_Eeprom_WritePage(page, offset, data_bytes, sizeof(float));
    return eeprom->write_page(page, offset, data_bytes, (uint16_t)(num_floats * sizeof(float)));
}

uint8_t
    App_Eeprom_ReadFloats(struct Eeprom *eeprom, uint16_t page, uint8_t offset, float *output_data, uint8_t num_floats)
{
    // Check if data to read fits into single page
    if (num_floats * sizeof(float) + offset > PAGE_SIZE)
    {
        return EEPROM_SIZE_ERROR;
    }

    uint8_t data[num_floats * sizeof(float)];
    uint8_t read_status;

    //    read_status = Io_Eeprom_ReadPage(page, offset, data, num_floats * sizeof(float));
    read_status = eeprom->read_page(page, offset, data, (uint16_t)(num_floats * sizeof(float)));

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_data[i] = convert_bytes_to_float(&data[i * sizeof(float)]);
    }

    return read_status;
}
