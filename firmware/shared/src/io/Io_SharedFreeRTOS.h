#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

/** @brief Required data structures for creating a static FreeRTOS queue */
struct StaticQueue
{
    StaticQueue_t state;
    uint8_t*      storage;
    QueueHandle_t handle;
};

/** @brief Required data structures for creating a static FreeRTOS semaphore*/
struct StaticSemaphore
{
    SemaphoreHandle_t handle;
    StaticSemaphore_t storage;
};
