#include "io_semaphore.hpp"
#include "cmsis_os2.h"

#include <cassert>

namespace io::semaphore
{
#ifdef TARGET_EMBEDDED

Semaphore::Semaphore(const bool priority_inheritance_protocol_on, const uint32_t sysview_id)
  : _sysview_id(sysview_id)
{
    if (priority_inheritance_protocol_on)
    {
        const osMutexAttr_t attr{
            .name      = nullptr,
            .attr_bits = osMutexPrioInherit,
            .cb_mem    = &freertos_semaphore_buf_,
            .cb_size   = sizeof(StaticSemaphore_t),
        };
        mutex_id_ = osMutexNew(&attr);
        assert(mutex_id_ != nullptr);
    }
    else
    {
        const osSemaphoreAttr_t attr{
            .name      = nullptr,
            .attr_bits = 0,
            .cb_mem    = &freertos_semaphore_buf_,
            .cb_size   = sizeof(StaticSemaphore_t),
        };
        semaphore_id_ = osSemaphoreNew(1, 1, &attr);
        assert(semaphore_id_ != nullptr);
    }
}

void Semaphore::give() const
{
    if (mutex_id_ != nullptr)
        osMutexRelease(mutex_id_);
    else
        osSemaphoreRelease(semaphore_id_);
    if (_sysview_id != 0)
        SEGGER_SYSVIEW_MarkStop(_sysview_id);
}
void Semaphore::take(const uint32_t timeout) const
{
    [[maybe_unused]] const osStatus_t status =
        mutex_id_ != nullptr ? osMutexAcquire(mutex_id_, timeout) : osSemaphoreAcquire(semaphore_id_, timeout);
    assert(status == osOK);
    if (_sysview_id != 0)
        SEGGER_SYSVIEW_MarkStart(_sysview_id);
}

bool Semaphore::is_held() const
{
    return mutex_id_ != nullptr && osMutexGetOwner(mutex_id_) == osThreadGetId();
}

#else

Semaphore::Semaphore(bool p, uint32_t sysview_id) : created_(true), _sysview_id(sysview_id) {}
void Semaphore::give() const {}
void Semaphore::take(uint32_t) const {}

#endif
} // namespace io::semaphore
