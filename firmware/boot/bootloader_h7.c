#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "hw_flash.h"
#include "hw_hal.h"
#include "hw_fdcan.h"
#include "queue.h"

#define NUM_OF_BUFF 2

extern CanHandle can;
const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can.hcan);
    return &can;
}

static uint8_t       flash_buf[FLASH_WORD_BYTES];
static uint32_t      fill_count = 0;
static QueueHandle_t flash_queue;
static FlashJob_t    job;

void bootloader_boardSpecific_setFlashQueue(QueueHandle_t queue)
{
    flash_queue = queue;
}

void bootloader_boardSpecific_program(uint32_t address, uint64_t data)
{
    uint32_t buffer_idx = address % FLASH_WORD_BYTES;
    memcpy(&flash_buf[buffer_idx], &data, sizeof(data));

    // On the STM32H733xx microcontroller, the smallest amount of memory you can
    // program at a time is 32 bytes (one "flash word"). This means we cannot
    // program the data from a single CAN message, since the CAN message has at most 8 bytes of data.
    // So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then
    // we can write the entire flash word. This implementation only works if the binary size
    // is a multiple of 32 bytes, or the buffer won't fill up for the last few bytes so won't be written
    // into flash. This is guaranteed by canup.
    if ((buffer_idx + sizeof(uint64_t)) == FLASH_WORD_BYTES)
    {
        job.addr = address / FLASH_WORD_BYTES * FLASH_WORD_BYTES;
        memcpy(job.data, flash_buf, FLASH_WORD_BYTES);
        xQueueSend(flash_queue, &job, portMAX_DELAY);
    }
}
