#include "io_semaphoreFake.h"

#include "app_utils.h"
#include <cassert>

extern "C"
{
    void io_semaphore_create(Semaphore *sem, const bool priority_inversion_on)
    {
        sem->created = true;
        UNUSED(priority_inversion_on);
    }

    void io_semaphore_give(const Semaphore *sem)
    {
        assert(sem->created);
    }

    void io_semaphore_take(const Semaphore *sem, const uint32_t timeout)
    {
        assert(sem->created);
        UNUSED(timeout);
    }
}