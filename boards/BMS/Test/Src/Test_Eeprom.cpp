#include <gtest/gtest.h>
#include <Test_Utils.h>

extern "C"
{
#include "App_Eeprom.h"
#include <fff.h>
}

#ifndef DEFINE_FFF_GLOBALS
DEFINE_FFF_GLOBALS;
#endif

#define MAX_FLOATS_PER_PAGE 4

namespace EepromTest
{
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, read_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, write_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, page_erase, uint16_t);

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

TEST_F(BmsEepromTest, test_float_converted_to_bytes_full_page)
{
    uint16_t page   = 1; // arbitrary value
    uint8_t  offset = 0; // arbitrary value

    uint8_t num_floats = MAX_FLOATS_PER_PAGE;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE] = { 1.0, 2.0, 3.0, 4.0 };

    App_Eeprom_WriteFloats(eeprom, page, offset, input_float_array, num_floats);

    // read_page_fake.arg2val holds the pointer to the float input converted to a byte array
    uint8_t byte_values[PAGE_SIZE];

    // capture converted byte array
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        byte_values[i] = write_page_fake.arg2_val[i];
    }

    // Unfortunately, cannot easily access the bytes converted back to floats due to FFF limitations

    // convert bytes-array back into floats
    float output_float_array[MAX_FLOATS_PER_PAGE];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_float_array[i] = convert_bytes_to_float(&byte_values[i * sizeof(float)]);
    }

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_float_converted_to_bytes_half_page)
{
    uint16_t page   = 1; // arbitrary value
    uint8_t  offset = 0; // arbitrary value

    uint8_t num_floats = MAX_FLOATS_PER_PAGE / 2;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE / 2] = { 1.0, 2.0 };

    App_Eeprom_WriteFloats(eeprom, page, offset, input_float_array, num_floats);

    // read_page_fake.arg2val holds the pointer to the float input converted to a byte array
    uint8_t byte_values[PAGE_SIZE / 2];

    // capture converted byte array
    for (int i = 0; i < (PAGE_SIZE / 2); i++)
    {
        byte_values[i] = write_page_fake.arg2_val[i];
    }

    // Unfortunately, cannot easily access the bytes converted back to floats due to FFF limitations
    // Because function used is static and located in App_Eeprom.c, need to make a copy of that static function in this
    // file This is NOT maintainable, any changes to convert_bytes_to_float must be added here as well.

    // convert bytes-array back into floats
    float output_float_array[num_floats];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_float_array[i] = convert_bytes_to_float(&byte_values[i * sizeof(float)]);
    }

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_float_converted_to_bytes_half_page_with_offset)
{
    uint16_t page   = 1; // arbitrary value
    uint8_t  offset = 4; // arbitrary value

    uint8_t num_floats = MAX_FLOATS_PER_PAGE / 2;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE / 2] = { 1.0, 2.0 };

    App_Eeprom_WriteFloats(eeprom, page, offset, input_float_array, num_floats);

    // read_page_fake.arg2val holds the pointer to the float input converted to a byte array
    uint8_t byte_values[PAGE_SIZE / 2];

    // capture converted byte array
    for (int i = 0; i < (PAGE_SIZE / 2); i++)
    {
        byte_values[i] = write_page_fake.arg2_val[i];
    }

    // Unfortunately, cannot easily access the bytes converted back to floats due to FFF limitations
    // Because function used is static and located in App_Eeprom.c, need to make a copy of that static function in this
    // file This is NOT maintainable, any changes to convert_bytes_to_float must be added here as well.

    // convert bytes-array back into floats
    float output_float_array[num_floats];

    for (uint8_t i = 0; i < num_floats; i++)
    {
        output_float_array[i] = convert_bytes_to_float(&byte_values[i * sizeof(float)]);
    }

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_size_error)
{
    uint16_t page   = 1;
    uint8_t  offset = 0;
    uint8_t  num_floats =
        MAX_FLOATS_PER_PAGE + 1; // PAGE_SIZE is 16 bytes, float size is 4 bytes, max floats per page is 4

    float input_data[num_floats];
    float output_data[num_floats];

    EEPROM_StatusTypeDef return_val_write = App_Eeprom_WriteFloats(eeprom, page, offset, input_data, num_floats);
    EEPROM_StatusTypeDef return_val_read  = App_Eeprom_ReadFloats(eeprom, page, offset, output_data, num_floats);

    ASSERT_EQ(return_val_read, EEPROM_SIZE_ERROR);
    ASSERT_EQ(return_val_write, EEPROM_SIZE_ERROR);
}

TEST_F(BmsEepromTest, test_invalid_offset)
{
    uint16_t page       = 1; // arbitrary value
    uint8_t  offset     = 3; // arbitrary value
    uint8_t  num_floats = 1;

    float input_data[num_floats];

    EEPROM_StatusTypeDef return_status = App_Eeprom_WriteFloats(eeprom, page, offset, input_data, num_floats);

    ASSERT_EQ(return_status, EEPROM_ADDR_ERROR);
}

} // namespace EepromTest
