#include "hw_flash.h"
#include "hw_hal.h"
#include <string.h>
#include "hw_utils.h"

#if defined(STM32F412Rx)

bool hw_flash_programByte(uint32_t address, uint8_t data)
{
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, data);
    HAL_FLASH_Lock();
    return status == HAL_OK;
}

bool hw_flash_programHalfWord(uint32_t address, uint16_t data)
{
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);
    HAL_FLASH_Lock();
    return status == HAL_OK;
}

bool hw_flash_programWord(uint32_t address, uint32_t data)
{
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
    HAL_FLASH_Lock();
    return status == HAL_OK;
}

bool hw_flash_program(uint32_t address, uint8_t *buffer, uint32_t size)
{
    bool status = true;
    HAL_FLASH_Unlock();

    for (uint32_t byte = 0; byte < size; byte++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address + byte, buffer[byte]) != HAL_OK)
        {
            status = false;
            break;
        }
    }

    HAL_FLASH_Lock();
    return status;
}

#elif defined(STM32H733xx)

bool hw_flash_programFlashWord(uint32_t address, uint32_t *data)
{
    const int         MAX_RETRIES = 3;
    HAL_StatusTypeDef status;

    HAL_FLASH_Unlock();
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++)
    {
        // clear flash errors from previous attempts
        if (attempt > 0)
        {
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
        }

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)data);

        if ((status == HAL_OK) && (memcmp((void *)address, data, FLASH_WORD_BYTES) == 0))
        {
            // verify contents in flash match what we programmed
            HAL_FLASH_Lock();
            return true;
        }

        // small delay between retries
        for (uint8_t i = 0; i < 100; i++)
        {
            __ASM("nop");
        }
    }
    HAL_FLASH_Lock();

    return false;
}

#endif

bool hw_flash_eraseSector(uint8_t sector)
{
    FLASH_EraseInitTypeDef erase = {
        .TypeErase    = FLASH_TYPEERASE_SECTORS,
        .Banks        = FLASH_BANK_1,
        .Sector       = sector,
        .NbSectors    = 1,
        .VoltageRange = FLASH_VOLTAGE_RANGE_3 // For device operating range 2.7V to 3.6V
    };

    uint32_t sector_error;
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&erase, &sector_error);
    HAL_FLASH_Lock();

    // Sector error = 0xFFFFFFFFU means that all the sectors have been correctly erased.
    return status == HAL_OK && sector_error == 0xFFFFFFFFU;
}
