#include "io_lowVoltageBattery.h"
#include "hw_i2c.h"
#include "hw_hal.h"

const uint8_t TARGET_ADDRESS = 0x10;
const uint8_t COMMAND_ADDRESS = 0x3E;

// const uint8_t CELL0_VOLTAGE_COMMAND[] = {0x14, 0x15};
// const uint8_t CELL1_VOLTAGE_COMMAND[] = {0x16, 0x17};
// const uint8_t CELL2_VOLTAGE_COMMAND[] = {0x1A, 0x1B};
// const uint8_t CELL4_VOLTAGE_COMMAND[] = {0x1C, 0x1D};
// const uint8_t STACK_VOLTAGE_COMMAND[] = {0x34, 0x35};

extern I2C_HandleTypeDef hi2c1;

static I2cInterface lvBatMon = { &hi2c1, TARGET_ADDRESS, 100 };

bool io_lowVoltageBattery_init()
{
    return hw_i2c_isTargetReady(&lvBatMon);
}

bool io_lowVoltageBattery_getPackVoltage(uint16_t *packVoltage)
{
    // 1 & 2. Write lower byte of command to 0x3E and upper byte of command to 0x3F
    uint8_t command_buffer[2];
    command_buffer[0] = 0x36;
    command_buffer[1] = 0x37;
    if (!hw_i2c_memWrite(&lvBatMon, COMMAND_ADDRESS, command_buffer, sizeof(command_buffer)))
    {
        return false;
    }

    // 3. Read 0x3E and 0x3F until readback returns original commands
    // NOTE: Is there a safer/better way to poll addresses 0x3E and 0x3F?
    // Loop should break and return false after waiting too long
    bool readDataReady = false;
    while (!readDataReady)
    {
        uint8_t rx_buffer[2];
        if (hw_i2c_memRead(&lvBatMon, COMMAND_ADDRESS, rx_buffer, sizeof(rx_buffer)))
        {
            if (rx_buffer[0] == command_buffer[0] && rx_buffer[1] == command_buffer[1])
            {
                readDataReady = true;
            }
        }
    }

    // 4. Read the length of response from 0x61
    uint8_t responseLength;
    if (!hw_i2c_memRead(&lvBatMon, 0x61, &responseLength, sizeof(responseLength)))
    {
        return false;
    }
    if (responseLength > sizeof(uint16_t))
    {
        return false; // Response length too large, not 16-bits
    }
    // 5. Read buffer starting at 0x40 using responseLength
    uint16_t voltageBuffer;
    if (!hw_i2c_memRead(&lvBatMon, 0x40, &voltageBuffer, responseLength))
    {
        return false;
    }
    // 6. Read the checksum at 0x60, if checkSum == data read, update packVoltage
    uint8_t checkSum;
    if (!hw_i2c_memRead(&lvBatMon, 0x60, &checkSum, sizeof(checkSum)))
    {
        return false;
    }
    if (checkSum != voltageBuffer) {
        return false;
    } else {
        *packVoltage = voltageBuffer;
    }
    return true;
}


