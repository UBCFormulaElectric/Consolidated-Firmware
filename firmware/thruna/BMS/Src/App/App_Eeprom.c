#include "App_Eeprom.h"
#include "string.h"

#define NUM_PAGES (128U)
#define SAVED_COPIES 4U
#define BYTES_PER_FLOAT sizeof(float) / sizeof(uint8_t)
#define BYTES_PER_SHORT sizeof(uint16_t) / sizeof(uint8_t)
#define DEFAULT_OFFSET 0U
#define ADDRESS_PAGE 0U

static void convert_float_to_bytes(uint8_t *byte_array, float float_to_convert)
{
    memcpy(byte_array, (uint8_t *)(&float_to_convert), sizeof(float));
}

static float convert_bytes_to_float(uint8_t *byte_array)
{
    float converted_float;
    memcpy(&converted_float, byte_array, sizeof(float));
    return converted_float;
}

static void convert_short_to_bytes(uint8_t *byte_array, uint16_t short_to_convert)
{
    memcpy(byte_array, (uint8_t *)(&short_to_convert), sizeof(uint16_t));
}

static uint16_t convert_bytes_to_short(uint8_t *bytes_to_convert)
{
    uint16_t converted_short;
    memcpy(&converted_short, bytes_to_convert, sizeof(uint16_t));
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
    EEPROM_StatusTypeDef read_status;

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

EEPROM_StatusTypeDef App_Eeprom_UpdateSavedAddress(struct Eeprom *eeprom, uint16_t current_address)
{
    uint16_t num_bytes = SAVED_COPIES * sizeof(uint16_t); // saving 3 copies of address, each 2 bytes
    uint8_t  byte_array[num_bytes];

    // Increment address for wear levelling purposes
    uint16_t new_address;
    if (current_address >= NUM_PAGES - 1)
    {
        new_address = DEFAULT_SOC_ADDR;
    }
    else
    {
        new_address = (uint16_t)(current_address + 1);
    }

    // convert address to bytes and write 4 copies to page 0

    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        convert_short_to_bytes(&byte_array[i * sizeof(uint16_t)], new_address);
    }

    return eeprom->write_page(ADDRESS_PAGE, DEFAULT_OFFSET, byte_array, num_bytes);
}

ExitCode App_Eeprom_ReadAddress(struct Eeprom *eeprom, uint16_t *address)
{
    if (*address >= NUM_PAGES)
    {
        return EXIT_CODE_ERROR;
    }

    uint16_t             num_bytes = SAVED_COPIES * sizeof(uint16_t); // saving 3 copies of address, each 2 bytes
    uint8_t              byte_array[num_bytes];
    uint16_t             address_array[SAVED_COPIES];
    EEPROM_StatusTypeDef read_status;

    // Read saved address
    read_status = eeprom->read_page(ADDRESS_PAGE, DEFAULT_OFFSET, byte_array, num_bytes);

    // Convert read bytes to shorts
    for (uint8_t i = 0; i < SAVED_COPIES; i++)
    {
        address_array[i] = convert_bytes_to_short(&byte_array[i * sizeof(uint16_t)]);
    }

    // Determine if read was successful and if address is uncorrupted
    if (read_status != EEPROM_OK)
    {
        *address = 1;
        return EXIT_CODE_ERROR;
    }
    else if (address_array[0] == address_array[1])
    {
        *address = address_array[0];
        return EXIT_CODE_OK;
    }
    else if (address_array[1] == address_array[2])
    {
        *address = address_array[1];
        return EXIT_CODE_OK;
    }
    else if (address_array[0] == address_array[2])
    {
        *address = address_array[0];
        return EXIT_CODE_OK;
    }
    else if (address_array[0] == address_array[3])
    {
        *address = address_array[0];
        return EXIT_CODE_OK;
    }
    else if (address_array[1] == address_array[3])
    {
        *address = address_array[1];
        return EXIT_CODE_OK;
    }
    else if (address_array[2] == address_array[3])
    {
        *address = address_array[2];
        return EXIT_CODE_OK;
    }
    else
    {
        *address = 1;
        return EXIT_CODE_ERROR;
    }
}

EEPROM_StatusTypeDef App_Eeprom_WriteMinSoc(struct Eeprom *eeprom, float min_soc, uint16_t address)
{
    float float_arr[SAVED_COPIES];

    for (uint8_t i = 0U; i < SAVED_COPIES; i++)
    {
        float_arr[i] = min_soc;
    }

    return App_Eeprom_WriteFloats(eeprom, address, DEFAULT_OFFSET, float_arr, SAVED_COPIES);
}

ExitCode App_Eeprom_ReadMinSoc(struct Eeprom *eeprom, uint16_t address, float *min_soc)
{
    float                float_arr[SAVED_COPIES];
    EEPROM_StatusTypeDef read_status = App_Eeprom_ReadFloats(eeprom, address, DEFAULT_OFFSET, float_arr, SAVED_COPIES);

    if (read_status != EEPROM_OK)
    {
        *min_soc = -1;
        return EXIT_CODE_ERROR;
    }
    else if (float_arr[0] == float_arr[1])
    {
        *min_soc = float_arr[0];
        return EXIT_CODE_OK;
    }
    else if (float_arr[1] == float_arr[2])
    {
        *min_soc = float_arr[1];
        return EXIT_CODE_OK;
    }
    else if (float_arr[0] == float_arr[2])
    {
        *min_soc = float_arr[0];
        return EXIT_CODE_OK;
    }
    else if (float_arr[0] == float_arr[3])
    {
        *min_soc = float_arr[0];
        return EXIT_CODE_OK;
    }
    else if (float_arr[1] == float_arr[3])
    {
        *min_soc = float_arr[1];
        return EXIT_CODE_OK;
    }
    else if (float_arr[2] == float_arr[3])
    {
        *min_soc = float_arr[2];
        return EXIT_CODE_OK;
    }
    else
    {
        *min_soc = -1;
        return EXIT_CODE_ERROR;
    }
}
