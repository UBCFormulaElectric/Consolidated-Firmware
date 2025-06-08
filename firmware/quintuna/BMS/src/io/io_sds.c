#include "io_sds.h"
#include "cmsis_os.h"
#include <stdbool.h>

#define SD_QUEUE_LENGTH 4

static osMessageQueueId_t sdcardQueueHandle = NULL;

void io_sds_queue_init(void)
{
    if (sdcardQueueHandle == NULL)
    {
        sdcardQueueHandle = osMessageQueueNew(SD_QUEUE_LENGTH, sizeof(SdRequest), NULL);
    }
}

bool io_sds_enqueue(SdRequest *req)
{
    if (sdcardQueueHandle == NULL)
        return false;
    return osMessageQueuePut(sdcardQueueHandle, req, 0, 0) == osOK;
}

osMessageQueueId_t io_sds_queue_handle(void)
{
    return sdcardQueueHandle;
}