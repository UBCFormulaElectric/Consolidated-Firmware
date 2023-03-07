#include <string.h>
#include "Io_Eeprom.h"
#include "Io_SharedI2c.h"
#include "App_SharedExitCode.h"
#include "App_SharedMacros.h"

#define BYTES_PER_WORD 4U
#define MAX_ADDRESS_BYTES 2048U
#define MAX_WRITE_SIZE_BYTES 15U

#define EEPROM_I2C_ADDRESS 0xA0
#define EEPROM_I2C_TIMEOUT_MS 1000

static SharedI2c i2c;

static uint8_t Io_Eeprom_GetI2cAddress(uint16_t address_bytes)
{
    uint8_t  i2c_address  = EEPROM_I2C_ADDRESS;

    // The 3 most significant bits of EEPROM byte address are added to the end of the I2C address
    i2c_address |= (uint8_t)(address_bytes >> 7);

    return i2c_address;
}

bool Io_Eeprom_WriteRaw(uint16_t address_bytes, void *data, uint8_t size_bytes)
{
    if (address_bytes + size_bytes > MAX_ADDRESS_BYTES)
    {
        return false;
    }

    bool status_ok = true;
    for(uint32_t txed_bytes = 0U; txed_bytes < size_bytes; txed_bytes += MAX_WRITE_SIZE_BYTES)
    {
        const uint8_t remaining_bytes = (uint8_t)(size_bytes - txed_bytes);
        const uint8_t transfer_size = MIN(remaining_bytes, MAX_WRITE_SIZE_BYTES);

        const uint8_t tx_buf_size = (uint8_t)(transfer_size + 1);
        uint8_t       tx_buf[tx_buf_size];

        uint8_t  address_lsb = (uint8_t)((address_bytes + txed_bytes) & 0xFFU);
        tx_buf[0] = address_lsb;
        memcpy(&tx_buf[1], (uint8_t*)data + txed_bytes, transfer_size);

        const uint8_t i2c_address = Io_Eeprom_GetI2cAddress((uint16_t)(address_bytes + txed_bytes));
        status_ok &= EXIT_OK(Io_SharedI2c_Transmit(&i2c, i2c_address, tx_buf, tx_buf_size));
    }

    return status_ok;
}

bool Io_Eeprom_ReadRaw(uint16_t address_bytes, void *data, uint8_t size_bytes)
{
    if (address_bytes + size_bytes > MAX_ADDRESS_BYTES)
    {
        return false;
    }

    const uint8_t i2c_address = Io_Eeprom_GetI2cAddress(address_bytes);
    uint8_t       address_lsb = (uint8_t)((address_bytes) & 0xFFU);

    return EXIT_OK(Io_SharedI2c_TransmitAndReceive(&i2c, i2c_address, &address_lsb, 1, data, size_bytes));
}

void Io_Eeprom_Init(I2C_HandleTypeDef *i2c_handle)
{
    Io_SharedI2c_Init(&i2c, i2c_handle, EEPROM_I2C_TIMEOUT_MS);
}

bool Io_Eeprom_WriteFloat(uint16_t address_words, float value)
{
    return Io_Eeprom_WriteRaw((uint16_t)(address_words * BYTES_PER_WORD), &value, BYTES_PER_WORD);
}

bool Io_Eeprom_ReadFloat(uint16_t address_words, float *value)
{
    return Io_Eeprom_ReadRaw((uint16_t)(address_words * BYTES_PER_WORD), value, BYTES_PER_WORD);
}
