#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "main.h"
#include <stm32f3xx_hal.h>
#include "App_SoC.h"
#include "eeprom_m24c16_fmn6tp.h"

#define SOC_NUM_OF_BYTES 4

static float_t SoC;
const float_t  w_SoC = 0.3;
// const float_t SoC = 123.5;

float_t App_SoC_getSoc(void)
{
    return SoC;
}

HAL_StatusTypeDef App_SoC_writeSoc(void)
{
    /**
    HAL_StatusTypeDef status;

    // Write SoC to 3 separate base addresses88
    if (Io_Eeprom_M24C16_writeToEeprom(
            0x00, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }
    if (Io_Eeprom_M24C16_writeToEeprom(
            0xa0, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }
    if (Io_Eeprom_M24C16_writeToEeprom(
            0xf0, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }
     **/

    HAL_StatusTypeDef status = HAL_OK;

    status = Io_Eeprom_M24C16_writeToEeprom(
        0x00, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES);
    if (status != HAL_OK)
    {
        return status;
    }

    status = Io_Eeprom_M24C16_writeToEeprom(
        0x00, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES);

    if (status != HAL_OK)
    {
        return status;
    }

    status = Io_Eeprom_M24C16_writeToEeprom(
        0x00, (uint8_t *)&w_SoC, SOC_NUM_OF_BYTES);
    if (status != HAL_OK)
    {
        return status;
    }

    return status;
}

void App_SoC_ReadSoC(void)
{
    float_t test_SoCs[3];
    bool    AB, BC, AC;

    // Test read from EEPROM
    if (Io_Eeprom_M24C16_readFromEeprom(
            0x00, (uint8_t *)&test_SoCs[0], SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }

    if (Io_Eeprom_M24C16_readFromEeprom(
            0xa0, (uint8_t *)&test_SoCs[1], SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }

    if (Io_Eeprom_M24C16_readFromEeprom(
            0xf0, (uint8_t *)&test_SoCs[2], SOC_NUM_OF_BYTES) != HAL_OK)
    {
        Error_Handler();
    }

    // Equality comparison from each mem address
    AB = (memcmp(
              (void *)&test_SoCs[0], (uint8_t *)&test_SoCs[1],
              SOC_NUM_OF_BYTES) == 0)
             ? true
             : false;
    BC = (memcmp(
              (void *)&test_SoCs[1], (uint8_t *)&test_SoCs[2],
              SOC_NUM_OF_BYTES) == 0)
             ? true
             : false;
    AC = (memcmp(
              (void *)&test_SoCs[0], (uint8_t *)&test_SoCs[2],
              SOC_NUM_OF_BYTES) == 0)
             ? true
             : false;

    // MLD Decision, and return pointer
    if (AB == true)
        SoC = test_SoCs[0];
    else if (BC == true)
        SoC = test_SoCs[1];
    else if (AC == true)
        SoC = test_SoCs[0];
}
