#include "hw_flash.h"
#include "hw_hal.h"
#include <stdint.h>
#include <string.h>
#include "hw_utils.h"

#define FLASH_MAX_RETRIES 3
static bool hw_flash_retryFlash(uint32_t address, uint32_t data, uint32_t size, uint32_t type);

#if defined(STM32F412Rx)

#define FLASH_ERROR_FLAGS                                                                               \
    (FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_OPERR | \
     FLASH_FLAG_RDERR)
// On F4: 'data' is a literal value, so compare flash against &data
#define FLASH_VERIFY_OK(address, data, size) (memcmp((void *)(address), &(data), (size)) == 0)

bool hw_flash_programByte(uint32_t address, uint8_t data)
{
    HAL_FLASH_Unlock();
    const bool ok = hw_flash_retryFlash(address, data, sizeof(data), FLASH_TYPEPROGRAM_BYTE);
    HAL_FLASH_Lock();
    return ok;
}

bool hw_flash_programHalfWord(uint32_t address, uint16_t data)
{
    HAL_FLASH_Unlock();
    const bool ok = hw_flash_retryFlash(address, data, sizeof(data), FLASH_TYPEPROGRAM_HALFWORD);
    HAL_FLASH_Lock();
    return ok;
}

bool hw_flash_programWord(uint32_t address, uint32_t data)
{
    HAL_FLASH_Unlock();
    bool ok = hw_flash_retryFlash(address, data, sizeof(data), FLASH_TYPEPROGRAM_WORD);
    HAL_FLASH_Lock();
    return ok;
}

bool hw_flash_program(uint32_t address, uint8_t *buffer, uint32_t size)
{
    bool status = true;
    HAL_FLASH_Unlock();

    for (uint32_t byte = 0; byte < size; byte++)
    {
        if (!hw_flash_retryFlash(address + byte, buffer[byte], 1, FLASH_TYPEPROGRAM_BYTE))
        {
            status = false;
            break;
        }
    }

    HAL_FLASH_Lock();
    return status;
}

#elif defined(STM32H733xx)

#define FLASH_ERROR_FLAGS (FLASH_FLAG_ALL_ERRORS_BANK1 | FLASH_FLAG_ALL_ERRORS_BANK2)
// On H7: 'data' is an address, so compare flash against the buffer
#define FLASH_VERIFY_OK(address, data, size) (memcmp((void *)(address), (void *)(data), (size)) == 0)

bool hw_flash_programFlashWord(uint32_t address, uint32_t *data)
{
    HAL_FLASH_Unlock();
    const bool ok = hw_flash_retryFlash(address, (uint32_t)data, FLASH_WORD_BYTES, FLASH_TYPEPROGRAM_FLASHWORD);
    HAL_FLASH_Lock();
    return ok;
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

/**
 * Retry wrapper around HAL_FLASH_Program with verification.
 *
 * @param address Destination flash address.
 * @param data  F4: literal flash value.
                H7: address to data buffer with flash values.
 * @param size Number of bytes expected to be written.
 * @param type HAL flash program type (BYTE/HALFWORD/WORD for F4, FLASHWORD for H7).
 *
 * @return true if programming and verification succeeded within retry limit,
 *         false otherwise.
 */
static bool hw_flash_retryFlash(uint32_t address, uint32_t data, uint32_t size, uint32_t type)
{
    HAL_StatusTypeDef status;

    for (int attempt = 0; attempt < FLASH_MAX_RETRIES; attempt++)
    {
        // clear flags from previous attempts
        if (attempt > 0)
        {
            __HAL_FLASH_CLEAR_FLAG(FLASH_ERROR_FLAGS);
        }

        status = HAL_FLASH_Program(type, address, data);

        // verify contents match what we programmed
        if ((status == HAL_OK) && FLASH_VERIFY_OK(address, data, size))
        {
            return true;
        }

        // small delay between retries
        for (uint8_t i = 0; i < 100; i++)
        {
            __ASM("nop");
        }
    }
    return false;
}
