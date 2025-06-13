#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef TARGET_EMBEDDED
#include "semphr.h"
#include <FreeRTOS.h>
#include <cmsis_os.h>
#include <cmsis_os2.h>
#include <portmacro.h>

typedef struct
{
    StaticSemaphore_t freertos_semaphore_buf;
    SemaphoreHandle_t freertos_semaphore;
} Semaphore;

#define MAX_TIMEOUT portMAX_DELAY
#else
typedef struct
{
    bool created;
} Semaphore;
#define MAX_TIMEOUT UINT32_MAX
#endif

void io_semaphore_create(const Semaphore *sem, bool priority_inversion_on);
void io_semaphore_take(const Semaphore *sem, uint32_t timeout);
void io_semaphore_give(const Semaphore *sem);
