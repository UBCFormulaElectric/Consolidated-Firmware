#pragma once

#include <stm32f3xx_hal.h>

/* Device Address Constants */
// static const uint16_t IMU_ADDR = 0x6b; // 0b110_1011

/* Register Address Constants */
#define TEST_ADDR 0x00

#include "stm32f3xx_hal.h"

/**
 * @brief      Initializes I2C Handler
 * @param      None
 * @return     None
 */
HAL_StatusTypeDef initI2CHandler(I2C_HandleTypeDef *i2c_handle);

/**
 * @brief      Write data to a byte address and reads back the data
 * @param      None
 * @return     None
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_testWriteRead(float data);

/**
 * @brief       Read data from the EEPROM
 * @param       None
 * @return      Status
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_readFromEeprom(
    uint16_t read_start_address,
    uint8_t *data,
    uint16_t data_size);

/**
 * @brief       Write data from the EEPROM
 * @param       Write address: Write Address
 * @return      Status
 */
HAL_StatusTypeDef Io_Eeprom_M24C16_writeToEeprom(
    uint16_t write_address,
    uint8_t *data,
    uint16_t data_size);

/**
 * @brief       Converts float to 4 byte packets
 * @param       stateOfCharge
 * @return      Ptr to 4 byte packets
 */
uint8_t *floatToIntByte(float stateOfCharge);

/**
 * @brief       Majority logic decision
 * @return      Ptr to 4 byte packets, else return null if error
 */
uint8_t *majorityLogicDecision();
