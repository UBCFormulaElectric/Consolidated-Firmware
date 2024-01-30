#include "test_bmsBaseStateMachineTest.h"

#define MAX_FLOATS_PER_PAGE 4

namespace EepromTest
{
static uint8_t static_byte_array[EEPROM_PAGE_SIZE];

// callback stores byte_array input into Io_Eeprom_WriteByte in static_byte_array to mimic writing to memory
static EepromStatus write_byte_callback(uint16_t page, uint8_t offset, uint8_t *byte_arr, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        static_byte_array[i] = byte_arr[i];
    }
    return EEPROM_STATUS_OK;
}

// callback stores copies stored static_byte_array into array pointed to in argument list of Io_Eeprom_ReadByte
// to mimic reading from memory
static EepromStatus read_byte_callback(uint16_t page, uint8_t offset, uint8_t *byte_arr, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        byte_arr[i] = static_byte_array[i];
    }
    return EEPROM_STATUS_OK;
}

class BmsEepromTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        fake_io_eeprom_readPage_reset();
        fake_io_eeprom_writePage_reset();
        fake_io_eeprom_pageErase_reset();
    }
};

TEST_F(BmsEepromTest, test_float_converted_to_bytes_full_page)
{
    uint16_t page       = 1; // arbitrary value
    uint8_t  offset     = 0; // arbitrary value
    uint8_t  num_floats = MAX_FLOATS_PER_PAGE;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE] = { 1.0f, 2.0f, 3.0f, 4.0f };

    fake_io_eeprom_writePage_setCallBack(write_byte_callback);
    app_eeprom_writeFloats(page, offset, input_float_array, num_floats);

    // convert bytes-array back into floats
    float output_float_array[MAX_FLOATS_PER_PAGE] = { 0 };

    fake_io_eeprom_readPage_setCallBack(read_byte_callback);
    app_eeprom_readFloats(page, offset, output_float_array, num_floats);

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_FLOAT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_float_converted_to_bytes_half_page)
{
    uint16_t page       = 1; // arbitrary value
    uint8_t  offset     = 0; // arbitrary value
    uint8_t  num_floats = MAX_FLOATS_PER_PAGE / 2;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE] = { 1.0f, 2.0f, 3.0f, 4.0f };

    fake_io_eeprom_writePage_setCallBack(write_byte_callback);
    app_eeprom_writeFloats(page, offset, input_float_array, num_floats);

    float output_float_array[MAX_FLOATS_PER_PAGE] = { 0 };

    fake_io_eeprom_readPage_setCallBack(read_byte_callback);
    app_eeprom_readFloats(page, offset, output_float_array, num_floats);

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_FLOAT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_float_converted_to_bytes_half_page_with_offset)
{
    uint16_t page       = 1; // arbitrary value
    uint8_t  offset     = 4; // arbitrary value
    uint8_t  num_floats = MAX_FLOATS_PER_PAGE / 2;

    // Fill float array with random numbers so that they are not garbage values
    float input_float_array[MAX_FLOATS_PER_PAGE] = { 1.0f, 2.0f, 3.0f, 4.0f };

    fake_io_eeprom_writePage_setCallBack(write_byte_callback);
    app_eeprom_writeFloats(page, offset, input_float_array, num_floats);

    float output_float_array[MAX_FLOATS_PER_PAGE] = { 0 };

    fake_io_eeprom_readPage_setCallBack(read_byte_callback);
    app_eeprom_readFloats(page, offset, output_float_array, num_floats);

    // Check that after converting floats to bytes and back their values are the same
    for (int i = 0; i < num_floats; i++)
    {
        ASSERT_FLOAT_EQ(input_float_array[i], output_float_array[i]);
    }
}

TEST_F(BmsEepromTest, test_size_error)
{
    uint16_t page       = 1;
    uint8_t  offset     = 0;
    uint8_t  num_floats = MAX_FLOATS_PER_PAGE + 1;

    float input_data[num_floats];
    float output_data[num_floats];

    EepromStatus return_val_write = app_eeprom_writeFloats(page, offset, input_data, num_floats);
    EepromStatus return_val_read  = app_eeprom_readFloats(page, offset, output_data, num_floats);

    ASSERT_EQ(return_val_read, EEPROM_STATUS_SIZE_ERROR);
    ASSERT_EQ(return_val_write, EEPROM_STATUS_SIZE_ERROR);
}

TEST_F(BmsEepromTest, test_invalid_offset)
{
    uint16_t page       = 1; // arbitrary value
    uint8_t  offset     = 3; // arbitrary value
    uint8_t  num_floats = 1;

    float input_data[num_floats];

    EepromStatus return_status = app_eeprom_writeFloats(page, offset, input_data, num_floats);

    ASSERT_EQ(return_status, EEPROM_STATUS_ADDR_ERROR);
}

} // namespace EepromTest
