#pragma GCC diagnostic ignored "-Wunused-variable"

//.h file to be included
#include "eeprom_m24c16_fmn6tp.h"
#include "string.h"
#include "stdbool.h"

// User PD
#define SOC_SIZE 4
#define WRITE_LOC 3

// I2C Handle for the I2C device the EEPROM is connected to
// May or may not need
static I2C_HandleTypeDef *eeprom_i2c_handle;

// The I2c slave device addresses to use when reading/writing to/from the EEPROM

static const uint16_t EEPROM_READ_ADDR  = 0xA1; // 0b101_00001
static const uint16_t EEPROM_WRITE_ADDR = 0xA0; // 0b101_00000

// Define test read and write addresses

#define TEST_ADDR 0x00

/**
 * Send the given data to the EEPROM
 * @param write_address Base address for write operation
 * @param data Data write
 * @param data_size Number of bytes to write
 * @return status
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_writeToEeprom(
    uint16_t write_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Write(
        eeprom_i2c_handle, EEPROM_WRITE_ADDR, write_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);
}

/**
 * Performs read at 4 separate locations
 * @param write_address The base EEPROM address we are reading from
 * @param *data Data received from EEPROM
 * @param SOC_SIZE, # of data points to read
 * @return status
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_readFromEeprom(
    uint16_t read_start_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Read(
        eeprom_i2c_handle, EEPROM_READ_ADDR, read_start_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);
}

/**
 * Configures I2C handler
 * @param *i2c_handle Ptr to hi2c1
 * @return
 */
HAL_StatusTypeDef initI2CHandler(I2C_HandleTypeDef *i2c_handle)
{
    eeprom_i2c_handle = i2c_handle;
    HAL_StatusTypeDef status;
    return status = HAL_OK;
}

/**
 * Splits floating point number into 4 bytes
 * @param stateOfCharge State of Charge
 * @return Ptr containing SoC array
 */
uint8_t *floatToIntByte(float stateOfCharge)
{
    static uint8_t SoCArray[SOC_SIZE];
    memcpy(SoCArray, &stateOfCharge, SOC_SIZE);
    return SoCArray;
}

/**
 * Writes to EEPROM then reads from EEPROM
 * @param SoC State of Charge (float)
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_testWriteRead(float SoC)
{
    uint8_t           dataRead[SOC_SIZE];
    uint8_t *         stateOfCharge;
    HAL_StatusTypeDef status = HAL_OK;

    // Convert floating point type SoC to bytes
    stateOfCharge = floatToIntByte(SoC);
    // uint8_t tmp0 = *(stateOfCharge + 0);
    // uint8_t tmp1 = *(stateOfCharge + 1);
    // uint8_t tmp2 = *(stateOfCharge + 2);
    // uint8_t tmp3 = *(stateOfCharge + 3);

    // Write SoC to 3 separate base addresses88
    status = Io_Eeprom_M24C16_writeToEeprom(0x00, stateOfCharge, SOC_SIZE);
    status = Io_Eeprom_M24C16_writeToEeprom(0xa0, stateOfCharge, SOC_SIZE);
    status = Io_Eeprom_M24C16_writeToEeprom(0xf0, stateOfCharge, SOC_SIZE);

    // Perform byte write
    // status = Io_Eeprom_M24C16_writeToEeprom(0x00,data,1);

    // Return if HAL_ERROR
    if (status != HAL_OK)
        return status;

    // Test read EEPROM
    // status = Io_Eeprom_M24C16_readFromEeprom(0x00, dataRead, SOC_SIZE);
    // status = Io_Eeprom_M24C16_readFromEeprom(0xa0, dataRead, SOC_SIZE);
    // status = Io_Eeprom_M24C16_readFromEeprom(0xf0, dataRead, SOC_SIZE);

    return status;
}

/**
 * Majority logic decision comparing data from three memory locations
 * return Returns the address of array containing SoC, else return null
 */
uint8_t *majorityLogicDecision()
{
    HAL_StatusTypeDef status;
    static uint8_t    SoC[WRITE_LOC][SOC_SIZE];
    bool              AB, BC, AC;

    // Stores EEPROM content at 3 separate locations
    status = Io_Eeprom_M24C16_readFromEeprom(0x00, SoC[0], SOC_SIZE);
    status = (status == HAL_ERROR)
                 ? HAL_ERROR
                 : Io_Eeprom_M24C16_readFromEeprom(0xa0, SoC[1], SOC_SIZE);
    status = (status == HAL_ERROR)
                 ? HAL_ERROR
                 : Io_Eeprom_M24C16_readFromEeprom(0xf0, SoC[2], SOC_SIZE);

    if (status == HAL_OK)
    {
        // Equality comparison from each mem address
        AB = (memcmp(SoC[0], SoC[1], SOC_SIZE) == 0) ? true : false;
        BC = (memcmp(SoC[1], SoC[2], SOC_SIZE) == 0) ? true : false;
        AC = (memcmp(SoC[0], SoC[2], SOC_SIZE) == 0) ? true : false;

        // MLD Decision, and return pointer
        if (AB == true)
            return SoC[0];
        else if (BC == true)
            return SoC[1];
        else if (AC == true)
            return SoC[0];
    }

    return NULL;
}
