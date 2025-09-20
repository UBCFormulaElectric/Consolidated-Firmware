#include "bootloader.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "hw_flash.h"
#include "hw_hal.h"
#include "hw_fdcan.h"
#include "queue.h"

extern CanHandle can;
const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can.hcan);
    return &can;
}

#define NUM_OF_BUFF 2

static uint8_t       flash_buf[NUM_OF_BUFF][FLASH_WORD_BYTES];
static uint8_t       active_buf = 0;
static uint32_t      fill_count = 0;
static uint32_t      base_addr[NUM_OF_BUFF];
static QueueHandle_t flash_queue;
static FlashJob      job;

void bootloader_boardSpecific_program(uint32_t address, uint64_t data)
{
    if (fill_count == 0)
    {
        base_addr[active_buf] = (address / FLASH_WORD_BYTES) * FLASH_WORD_BYTES;
    }

    memcpy(&flash_buf[active_buf][fill_count], &data, sizeof(data));
    fill_count += sizeof(data);

    if (fill_count == FLASH_WORD_BYTES)
    {
        job.addr = base_addr[active_buf];
        memcpy(job.data, flash_buf[active_buf], FLASH_WORD_BYTES);
        xQueueSend(flash_queue, &job, portMAX_DELAY);

        active_buf ^= 1;
        fill_count = 0;
        memset(flash_buf[active_buf], 0xFF, FLASH_WORD_BYTES);
    }
}
