#include "io/semaphore.h"

#include <assert.h>
#include <stddef.h>

void io_semaphore_create(Semaphore *sem, const bool priority_inversion_on)
{
    if (priority_inversion_on)
    {
        sem->freertos_semaphore = xSemaphoreCreateMutexStatic(&sem->freertos_semaphore_buf);
    }
    else
    {
        sem->freertos_semaphore = xSemaphoreCreateBinaryStatic(&sem->freertos_semaphore_buf);
    }
    assert(sem->freertos_semaphore != NULL);
    io_semaphore_give(sem);
}

void io_semaphore_give(const Semaphore *sem)
{
    xSemaphoreGive(sem->freertos_semaphore);
}
void io_semaphore_take(const Semaphore *sem, const uint32_t timeout)
{
    xSemaphoreTake(sem->freertos_semaphore, timeout);
}