#include "hw_flash.h"
#include "hw_hal.h"
#include <string.h>
#include "hw_utils.h"
#include "semphr.h"
#include "queue.h"
#include "io_log.h"

static osSemaphoreId_t    flashDoneSem;
static osMessageQueueId_t flashQueue;
static volatile bool      flash_busy;

/* --------- HAL CB --------- */
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
{
    flash_busy = false;
    HAL_FLASH_Lock();
    xSemaphoreGiveFromISR(flashDoneSem, NULL);
}

void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
    LOG_ERROR("Error while flashing");
    flash_busy = false;
    HAL_FLASH_Lock();
    xSemaphoreGiveFromISR(flashDoneSem, NULL);
}

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
    if (flash_busy)
    {
        return false;
    }

    flash_busy = true;
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)data);
    HAL_FLASH_Lock();

    return (status == HAL_OK);
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

void hw_flash_init(osSemaphoreId_t sem, osMessageQueueId_t queue)
{
    flashDoneSem = sem;
    flashQueue   = queue;
}

osMessageQueueId_t hw_flash_getFlashQueue(void)
{
    return flashQueue;
}

bool hw_flash_waitFlashComplete(TickType_t timeout)
{
    return xSemaphoreTake(flashDoneSem, timeout) == pdTRUE;
}
