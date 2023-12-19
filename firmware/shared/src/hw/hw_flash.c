#include "hw_flash.h"
#include "hw_hal.h"
#include <string.h>
#include "Io_SharedMacros.h"

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

bool hw_flash_programByte(uint32_t address, uint8_t data)
{
    return false;
}

bool hw_flash_programHalfWord(uint32_t address, uint16_t data)
{
    return false;
}

bool hw_flash_program(uint32_t address, uint8_t *buffer, uint32_t size)
{
    return false;
}

bool hw_flash_programWord(uint32_t address, uint32_t data)
{
    if (address == 0x8000000 + 3080)
    {
        BREAK_IF_DEBUGGER_CONNECTED();
    }

    uint32_t word_aligned_address = (address / sizeof(uint32_t)) * sizeof(uint32_t);
    uint32_t flash_word_aligned_address =
        (word_aligned_address / (4 * FLASH_NB_32BITWORD_IN_FLASHWORD)) * FLASH_NB_32BITWORD_IN_FLASHWORD * 4;
    uint32_t word_offset = (word_aligned_address / sizeof(uint32_t)) % FLASH_NB_32BITWORD_IN_FLASHWORD;

    uint32_t flash_word_data[FLASH_NB_32BITWORD_IN_FLASHWORD];
    memcpy(flash_word_data, (uint32_t *)flash_word_aligned_address, sizeof(flash_word_data));
    memcpy(&flash_word_data[word_offset], &data, sizeof(uint32_t));

    HAL_StatusTypeDef status1 = HAL_FLASH_Unlock();
    if (status1 != HAL_OK)
    {
        BREAK_IF_DEBUGGER_CONNECTED();
    }

    HAL_StatusTypeDef status2 =
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, flash_word_aligned_address, (uint32_t)flash_word_data);
    if (status2 != HAL_OK)
    {
        BREAK_IF_DEBUGGER_CONNECTED();
    }

    HAL_StatusTypeDef status3 = HAL_FLASH_Unlock();
    if (status3 != HAL_OK)
    {
        BREAK_IF_DEBUGGER_CONNECTED();
    }

    return true; // status == HAL_OK;
}

bool hw_flash_programFlashWord(uint32_t address, uint32_t* data)
{
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)data);
    HAL_FLASH_Lock();
    return status == HAL_OK;
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
