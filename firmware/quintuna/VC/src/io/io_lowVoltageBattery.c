#include "io_lowVoltageBattery.h"
#include "hw_i2c.h"
#include "hw_hal.h"

const uint8_t  BQ76922_I2C_ADDR      = 0x10;   // 7-bit I2C address
const uint16_t CMD_DASTATUS6         = 0x0076; // Subcommand for accumulated charge
const uint8_t  COMMAND_ADDRESS       = 0x3E;
const uint8_t  REG_SUBCOMMAND_LSB    = 0x3E;
const uint8_t  REG_SUBCOMMAND_MSB    = 0x3F;
const uint8_t  REG_DATA_BUFFER       = 0x40;
const uint8_t  REG_CHECKSUM          = 0x60;
const uint8_t  REG_RESPONSE_LENGTH   = 0x61;
const uint16_t CELL0_VOLTAGE_COMMAND = 0x1514;
const uint16_t CELL1_VOLTAGE_COMMAND = 0x1716;
const uint16_t CELL2_VOLTAGE_COMMAND = 0x1B1A;
const uint16_t CELL4_VOLTAGE_COMMAND = 0x1D1C;
const uint16_t STACK_VOLTAGE_COMMAND = 0x3534;

#define R_SENSE 2.0f    // Sense resistor in mΩ
#define Q_FULL 11200.0f // Battery full charge capacity in mAh

extern I2C_HandleTypeDef hi2c1;

static I2cInterface lvBatMon = { &hi2c1, BQ76922_I2C_ADDR, 100 };

bool io_lowVoltageBattery_init()
{
    return hw_i2c_isTargetReady(&lvBatMon);
}

bool writeSubcommand(uint16_t subcommand)
{
    uint8_t data[2] = { subcommand & 0xFF, (subcommand >> 8) & 0xFF };
    if (!hw_i2c_memWrite(&hi2c1, REG_SUBCOMMAND_LSB, data, 2))
    {
        return false;
    }

    return true;
}

uint8_t readResponseLength()
{
    uint8_t responseLength;
    if (!hw_i2c_memRead(&lvBatMon, REG_RESPONSE_LENGTH, &responseLength, sizeof(responseLength)))
    {
        return 0;
    }
    if (responseLength > sizeof(uint8_t))
    {
        return 0;
    }
    return responseLength;
}

bool readCharge(uint32_t *charge, uint16_t *time)
{
    uint8_t responseLen = readResponseLength();

    if (responseLen != 6)
    {
        return false;
    }

    uint8_t buffer[6];

    if (!hw_i2c_memRead(&hi2c1, REG_DATA_BUFFER, buffer, 6))
    {
        return false;
    }

    uint8_t checksum;
    if (!hw_i2c_memRead(&hi2c1, REG_CHECKSUM, &checksum, 1))
    {
        return false;
    }

    uint8_t calculated_checksum = (CMD_DASTATUS6 & 0xFF) + (CMD_DASTATUS6 >> 8) + responseLen;
    for (int i = 0; i < responseLen; i++)
    {
        calculated_checksum += buffer[i];
    }
    calculated_checksum = ~calculated_checksum; // Invert bits

    if (calculated_checksum != checksum)
    {
        return false;
    }

    *charge = (buffer[0] | (buffer[1] << 8) | (buffer[2] << 16));
    *time   = (buffer[3] | (buffer[4] << 8));

    return true;
}

float calculateSOC(uint32_t charge)
{
    float CC_GAIN    = 7.4768f / R_SENSE;
    float charge_mAh = (charge * CC_GAIN) / 3600.0f;
    return (charge_mAh / Q_FULL) * 100.0f;
}

bool io_lowVoltageBattery_getPackCharge(float *SOC)
{
    uint32_t *charge;
    uint16_t *time;

    if (!writeSubcommand(CMD_DASTATUS6))
    {
        return false;
    }

    if (!readCharge(charge, time))
    {
        return false;
    }

    *SOC = calculateSOC(charge);

    return true;
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
    if (checkSum != voltageBuffer)
    {
        return false;
    }
    else
    {
        *packVoltage = voltageBuffer;
    }
    return true;
}
