#pragma once
#include "cmsis_os.h"
#include <stdbool.h>

typedef enum
{
    SD_REQ_READ_SOC,
    SD_REQ_WRITE_SOC
} SdRequestType;

typedef struct
{
    SdRequestType   type;
    float           soc_value;
    float          *result_ptr;
    osSemaphoreId_t done_sem;
    bool           *success_ptr;
} SdRequest;

void               io_sds_queue_init(void);
bool               io_sds_enqueue(SdRequest *req);
osMessageQueueId_t io_sds_queue_handle(void);
