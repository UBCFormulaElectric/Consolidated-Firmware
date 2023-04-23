#include "Test_Eeprom.h"

extern "C"
{
#include "App_Eeprom.h"
}

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

namespace EepromTest
{
FAKE_VALUE_FUNC(uint8_t, read_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(uint8_t, write_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(uint8_t, page_erase, uint16_t);

class BmsEepromTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        eeprom = App_Eeprom_Create(write_page, read_page, page_erase);

        RESET_FAKE(read_page);
        RESET_FAKE(write_page);
        RESET_FAKE(page_erase);

        read_page_fake.return_val  = EEPROM_OK;
        write_page_fake.return_val = EEPROM_OK;
        page_erase_fake.return_val = EEPROM_OK;
    }

    void TearDown() override { TearDownObject(eeprom, App_Eeprom_Destroy); }

    struct Eeprom *eeprom;
};

TEST_F(BmsEepromTest, test_floats_correctly_converted)
{
    uint16_t page       = 1;
    uint8_t  offset     = 0;
    uint8_t  num_floats = 4; // PAGE_SIZE is 16 bytes, float size is 4 bytes, max floats per page is 4

    float input_float_array[4] = { 1.0, 2.0, 3.0, 4.0 };

    App_Eeprom_WriteFloats(eeprom, page, offset, input_float_array, num_floats);

    // read_page_fake.arg2val holds the pointer to the float input converted to a byte array
    // write_page_fake.arg2val holds the pointer to the byte-array input converted back into a float
    // this hardcoding is equivalent to writing and reading from the same page.
    read_page_fake.arg2_val = write_page_fake.arg2_val;

    float output_float_array[4] = { 0.0, 0.0, 0.0, 0.0 };

    App_Eeprom_ReadFloats(eeprom, page, offset, output_float_array, num_floats);

    for (int i = 0; i < 4; i++)
    {
        ASSERT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_size_error)
{
    uint16_t page   = 1;
    uint8_t  offset = 0;
    uint8_t  num_floats =
        (PAGE_SIZE / sizeof(float)) + 1; // PAGE_SIZE is 16 bytes, float size is 4 bytes, max floats per page is 4

    float input_data[num_floats];
    float output_data[num_floats];

    uint8_t return_val_write = App_Eeprom_WriteFloats(eeprom, page, offset, input_data, num_floats);
    uint8_t return_val_read  = App_Eeprom_ReadFloats(eeprom, page, offset, output_data, num_floats);

    ASSERT_EQ(return_val_read, EEPROM_SIZE_ERROR);
    ASSERT_EQ(return_val_write, EEPROM_SIZE_ERROR);
}

} // namespace EepromTest
