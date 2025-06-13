#include "io_semaphoreFake.h"

extern "C"
{
    void io_semaphore_create(const Semaphore *sem)
    {
        const_cast<Semaphore *>(sem)->created = true;
    }

    void io_semaphore_give(const Semaphore *sem)
    {
        assert(sem->created);
    }

    void io_semaphore_take(const Semaphore *sem)
    {
        assert(sem->created);
    }
}