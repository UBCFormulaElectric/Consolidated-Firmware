#pragma once
#include "cmsis_os.h"
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#else
#include "app_utils.h"
EMPTY_STRUCT(SdCard);
#endif

/**
 * Return whether or not a SD Card is present
 * @param SdGpio instance
 * @return true if SD card is present
 */
bool io_sds_checkSdPresent(void);

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
bool io_sds_readSocFromSd(float *saved_soc_c);
bool io_sds_writeSocToSd(float soc);
