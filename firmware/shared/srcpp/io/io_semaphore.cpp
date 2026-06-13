#include "io_semaphore.hpp"
#include "cmsis_os2.h"

#include <cassert>

#ifdef TARGET_EMBEDDED

io::semaphore::semaphore(const bool priority_inheritance_protocol_on, const uint32_t sysview_id)
  : _sysview_id(sysview_id)
{
    if (priority_inheritance_protocol_on)
    {
        freertos_semaphore_ = xSemaphoreCreateMutexStatic(&freertos_semaphore_buf_);
    }
    else
    {
        freertos_semaphore_ = xSemaphoreCreateBinaryStatic(&freertos_semaphore_buf_);
    }

    assert(freertos_semaphore_ != nullptr);
    give();
}

void io::semaphore::give() const
{
    // TODO find out why this is hanging
    // if (osKernelGetState() != taskSCHEDULER_RUNNING)
    //     return;
    xSemaphoreGive(freertos_semaphore_);
    if (_sysview_id != 0)
        SEGGER_SYSVIEW_MarkStop(_sysview_id);
}
void io::semaphore::take(const uint32_t timeout) const
{
    // TODO find out why this is hanging
    // if (osKernelGetState() != taskSCHEDULER_RUNNING)
    //     return;
    const auto take_success = xSemaphoreTake(freertos_semaphore_, timeout);
    assert(take_success == pdTRUE);
    if (_sysview_id != 0)
        SEGGER_SYSVIEW_MarkStart(_sysview_id);
}

bool io::semaphore::is_held() const
{
    return xSemaphoreGetMutexHolder(freertos_semaphore_) == xTaskGetCurrentTaskHandle();
}

#else

io::semaphore::semaphore(bool p, uint32_t sysview_id) : created_(true), _sysview_id(sysview_id) {}
void io::semaphore::give() const {}
void io::semaphore::take(uint32_t) const {}

#endif